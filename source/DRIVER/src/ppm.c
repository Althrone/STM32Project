#include "ppm.h"

//整个全局变量
PPM_DemodTimeTypeDef PPM_DemodTimeStruct={0,0,0,0,0,8};//顺便把数据缓冲初始化了
PPM_DemodFlagTypeDef PPM_DemodFlagStruct={0,0};
PPM_RawDataTypeDef PPM_RawDataStruct;

/**
 * PPM-PE12
 * 接收机发出的PPM信号精度为20us，根据香农定理，采样频率应该是它的两倍，
 * 也就是10us
 **/
void PPM_Init(void)
{
    //开GPIOE时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
    //GPIO初始化结构体定义
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_12;
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_DOWN;
    GPIO_Init(GPIOE,&GPIO_InitStruct);

    //使能SYSCFG时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, GPIO_PinSource12);//连接引脚与中断线

    //初始化中断线12
    EXTI_InitTypeDef EXTI_InitStruct;
    EXTI_InitStruct.EXTI_Line=EXTI_Line12;
    EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Rising;//上升沿触发中断
    EXTI_InitStruct.EXTI_LineCmd=ENABLE;
    EXTI_Init(&EXTI_InitStruct);

    //PPM接收机外部中断
    //定义NVIC初始化结构体
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel=EXTI15_10_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
    NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    TIM7_Init();
}

/**
 * @brief   从全局变量获取到一个结构体，用于导出参数的值
 * @param   ANO_DT_SendRCDataStruct: 发送给上位机的遥控器参数结构体
 **/
void PPM_GetRCData(ANO_DT_SendRCDataTypeDef* ANO_DT_SendRCDataStruct)
{
    ANO_DT_SendRCDataStruct->ANO_DT_RCRoll=PPM_RawDataStruct.PPM_RawCh1;
    ANO_DT_SendRCDataStruct->ANO_DT_RCPitch=PPM_RawDataStruct.PPM_RawCh2;
    ANO_DT_SendRCDataStruct->ANO_DT_RCThrottle=PPM_RawDataStruct.PPM_RawCh3;
    ANO_DT_SendRCDataStruct->ANO_DT_RCYaw=PPM_RawDataStruct.PPM_RawCh4;
    ANO_DT_SendRCDataStruct->ANO_DT_RCAUX1=PPM_RawDataStruct.PPM_RawCh5;//SWA
    ANO_DT_SendRCDataStruct->ANO_DT_RCAUX2=PPM_RawDataStruct.PPM_RawCh6;//SWB
    ANO_DT_SendRCDataStruct->ANO_DT_RCAUX3=PPM_RawDataStruct.PPM_RawCh7;//SWC
    ANO_DT_SendRCDataStruct->ANO_DT_RCAUX4=PPM_RawDataStruct.PPM_RawCh8;//SWD
}

/**
 * 
 * 
 **/
void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line12)!=RESET)//PWM输入发生触发
    {
        EXTI_ClearITPendingBit(EXTI_Line12);
        //计算时间片
        PPM_DemodTimeStruct.PPM_LastTime=PPM_DemodTimeStruct.PPM_NewTime;
        PPM_DemodTimeStruct.PPM_NewTime=1000*PPM_DemodTimeStruct.PPM_1msCnt+TIM7->CNT;//单位us
        PPM_DemodTimeStruct.PPM_TimeSlce=PPM_DemodTimeStruct.PPM_NewTime-PPM_DemodTimeStruct.PPM_LastTime;

        //前一次PMM序列已经传输完毕，这次触发就是新的PMM序列
        if((PPM_DemodTimeStruct.PPM_TimeSlce>=800)&&
           (PPM_DemodTimeStruct.PPM_TimeSlce<=2200))
        {
            if(PPM_DemodFlagStruct.PPM_EndFlag==SET)
            {
                PPM_DemodFlagStruct.PPM_EndFlag=RESET;
                PPM_DemodFlagStruct.PPM_WorkFlag=SET;
            }
            if(PPM_DemodFlagStruct.PPM_WorkFlag==SET)
            {
                if(PPM_DemodTimeStruct.PPM_TimeSlce<1000)
                PPM_DemodTimeStruct.PPM_TimeSlce=1000;
                else if(PPM_DemodTimeStruct.PPM_TimeSlce>2000)
                PPM_DemodTimeStruct.PPM_TimeSlce=2000;
                *(&PPM_RawDataStruct.PPM_RawCh1+PPM_DemodTimeStruct.PPM_PWMCnt)=PPM_DemodTimeStruct.PPM_TimeSlce;
                if(PPM_DemodTimeStruct.PPM_PWMCnt>=PPM_DemodTimeStruct.PPM_PWMMax)//对应通道数的参数传递完了
                {
                    PPM_DemodFlagStruct.PPM_EndFlag=SET;
                    PPM_DemodTimeStruct.PPM_PWMCnt=0;
                }
                else
                    PPM_DemodTimeStruct.PPM_PWMCnt++;
            }
            else//WorkFlag没有置1，就是说这段PPM不完整
            {}
        }
        //两脉冲之间时间超过2ms，代表这段PMM序列已经结束了
        else if(PPM_DemodTimeStruct.PPM_TimeSlce>2500)
        {
            PPM_DemodTimeStruct.PPM_1msCnt=0;
            PPM_DemodTimeStruct.PPM_LastTime=0;
            PPM_DemodTimeStruct.PPM_NewTime=0;
            PPM_DemodTimeStruct.PPM_TimeSlce=0;
            PPM_DemodTimeStruct.PPM_PWMCnt=0;
            PPM_DemodFlagStruct.PPM_EndFlag=SET;//PMM结束标志置1
            PPM_DemodFlagStruct.PPM_WorkFlag=RESET;
            TIM7->CNT=0;//TIM7也重新计时
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
        PPM_DemodTimeStruct.PPM_1msCnt++;
    }
}