#include "ppm.h"

//整个全局变量
PMM_DemodulateTypeDef PMM_DemodulateStructure={0,0,0,0,{0}};//顺便把数据缓冲初始化了

/**
 * PPM-TIM1_CH3N-PE12
 * 接收机发出的PPM信号精度为20us，根据香农定理，采样频率应该是它的两倍，
 * 也就是10us
 **/
void PPM_Init(void)
{
    //开GPIOE时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
    //GPIO初始化结构体定义
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;
    GPIO_Init(GPIOE,&GPIO_InitStructure);

    //使能SYSCFG时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, GPIO_PinSource12);//连接引脚与中断线

    //初始化中断线12
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line12;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//上升沿触发中断
    EXTI_InitStructure.EXTI_LineCmd	= ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    //PPM接收机外部中断
    //定义NVIC初始化结构体
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM7_Init();
}

/**
 * 
 * 
 **/
void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line12)!=RESET)//PWM输入发生触发
    {
        //计算时间片
        PMM_DemodulateStructure.PMM_LastTime=PMM_DemodulateStructure.PMM_NewTime;
        PMM_DemodulateStructure.PMM_NewTime=1000*PMM_DemodulateStructure.PMM_1msCnt+TIM5->CNT;//单位us
        PMM_DemodulateStructure.PMM_TimeSlce=PMM_DemodulateStructure.PMM_NewTime-PMM_DemodulateStructure.PMM_LastTime;
        //前一次PMM序列已经传输完毕，这次触发就是新的PMM序列
        if(PMM_DemodulateStructure.PMM_EndFlag==SET)
        {
            if((PMM_DemodulateStructure.PMM_TimeSlce>=1000)&&
               (PMM_DemodulateStructure.PMM_TimeSlce<=2000))
               {

               }
        }
        //两脉冲之间时间超过2ms，代表这段PMM序列已经结束了
        if(PMM_DemodulateStructure.PMM_TimeSlce>=2000)
        {
            PMM_DemodulateStructure.PMM_1msCnt=RESET;//1ms统计归零
            PMM_DemodulateStructure.PMM_EndFlag=SET;//PMM结束标志置1
        }
    }
}

/**
 * 定时1ms的记录
 **/
void TIM7_IRQHandler(void)
{
    if(TIM_GetFlagStatus(TIM7,TIM_FLAG_Update)!=RESET)
    {
        TIM_ClearFlag(TIM7,TIM_FLAG_Update);
        PMM_DemodulateStructure.PMM_1msCnt++;
    }
}