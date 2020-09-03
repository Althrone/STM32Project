#include "tim.h"

/* I2C public functions ------------------------------------------------------*/ 

/* I2C1 private functions ----------------------------------------------------*/ 

/**
 * TIM3用作电机PWM输出
 * 20ms周期
 * PWMOUT_1-TIM3_CH4-PB1
 * PWMOUT_2-TIM3_CH3-PB0
 * PWMOUT_3-TIM3_CH2-PA7
 * PWMOUT_4-TIM3_CH1-PA6
 **/
void TIM3_Init(void)
{
    //开TIM3时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    //TIM初始化时基单元结构体定义
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_Prescaler=42-1;//4分频就是4-1
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period=40000-1;
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//暂时不知道有什么用
    // TIM_TimeBaseInitStructure.TIM_RepetitionCounter=;//只有TIM1和TIM8才有
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);

    //TIM初始化输出比较结构体定义
    TIM_OCInitTypeDef TIM_OCInitStruct;
    //PWMOUT_1
    TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
    // TIM_OCInitStruct.TIM_OutputNState=;
    TIM_OCInitStruct.TIM_Pulse=0;//比较值
    TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
    // TIM_OCInitStruct.TIM_OCNPolarity=;
    // TIM_OCInitStruct.TIM_OCIdleState=;
    // TIM_OCInitStruct.TIM_OCNIdleState=;
    TIM_OC1Init(TIM3,&TIM_OCInitStruct);
    //PWMOUT_2
    TIM_OC2Init(TIM3,&TIM_OCInitStruct);
    //PWMOUT_3
    TIM_OC3Init(TIM3,&TIM_OCInitStruct);
    //PWMOUT_4
    TIM_OC4Init(TIM3,&TIM_OCInitStruct);
    //寄存器预装载
    TIM_ARRPreloadConfig(TIM3, ENABLE);
    //TIM3使能
    TIM_Cmd(TIM3,ENABLE);
}

void TIM5_Init(void)
{
    //开TIM5时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
    //TIM初始化时基单元结构体定义
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_Prescaler=42-1;//4分频就是4-1
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period=40000-1;
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//暂时不知道有什么用
    // TIM_TimeBaseInitStructure.TIM_RepetitionCounter=;//只有TIM1和TIM8才有
    TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);
}

/**
 * 试试定时1s
 * 
 **/
void TIM6_Init(void)
{
    //开TIM6时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
    //TIM初始化结构体定义
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_Prescaler=8400;
    // TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//TIM6只有向上计数模式
    TIM_TimeBaseInitStructure.TIM_Period=10000;
    // TIM_TimeBaseInitStructure.TIM_ClockDivision=;//TIM6没有时钟分频
    // TIM_TimeBaseInitStructure.TIM_RepetitionCounter=;//只有TIM1和TIM8才有
    TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure);

    //NVI初始化结构体定义
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel=TIM6_DAC_IRQn ; //定时器6中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //开TIM6中断
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
    //TIM6使能
    TIM_Cmd(TIM6,ENABLE);
}

/**
 * 定时1ms，然后通过获取当前值来细化时间
 * 用于精确定时，可细化到1us
 **/
void TIM7_Init(void)
{
    //开TIM7时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_Prescaler=84;
    TIM_TimeBaseInitStructure.TIM_Period=1000;
    TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStructure);

    //定义NVIC初始化结构体
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel=TIM7_IRQn; //定时器5中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1; //子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

    //开TIM6中断
    TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);
    //TIM6使能
    TIM_Cmd(TIM7,ENABLE);
}



/**
 * 用于驱动LED1s闪烁
 **/
uint8_t flag=0;

void TIM6_DAC_IRQHandler(void)
{
    if(TIM_GetFlagStatus(TIM6,TIM_FLAG_Update)!=RESET)
    {
        flag=~flag;
        if(flag==0)
        {
            RGBLED_OFF();
        }
        else
            RGBLED_White();
        TIM_ClearFlag(TIM6,TIM_FLAG_Update);
    }
}