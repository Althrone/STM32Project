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
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
    //PWMOUT_2
    TIM_OC2Init(TIM3,&TIM_OCInitStruct);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
    //PWMOUT_3
    TIM_OC3Init(TIM3,&TIM_OCInitStruct);
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
    //PWMOUT_4
    TIM_OC4Init(TIM3,&TIM_OCInitStruct);
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
    //寄存器预装载
    TIM_ARRPreloadConfig(TIM3, ENABLE);
    //TIM3使能
    TIM_Cmd(TIM3,ENABLE);
}

/**
 * @brief   舵机pwm
 * PWMOUT_5-TIM5_CH4-PA2
 * PWMOUT_6-TIM5_CH3-PA3
 **/
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

    //TIM初始化输出比较结构体定义
    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
    // TIM_OCInitStruct.TIM_OutputNState=;
    TIM_OCInitStruct.TIM_Pulse=0;//比较值
    TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
    // TIM_OCInitStruct.TIM_OCNPolarity=;
    // TIM_OCInitStruct.TIM_OCIdleState=;
    // TIM_OCInitStruct.TIM_OCNIdleState=;
    //PWMOUT_5
    TIM_OC4Init(TIM5,&TIM_OCInitStruct);
    TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);
    //PWMOUT_6
    TIM_OC3Init(TIM5,&TIM_OCInitStruct);
    TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);
    //寄存器预装载
    TIM_ARRPreloadConfig(TIM5, ENABLE);
    //TIM3使能
    TIM_Cmd(TIM5,ENABLE);
}

/**
 * 定时10ms
 * LED和数据采集
 **/
void TIM6_Init(void)
{
    //开TIM6时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
    //TIM初始化结构体定义
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_Prescaler=8400-1;
    // TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//TIM6只有向上计数模式
    TIM_TimeBaseInitStructure.TIM_Period=100-1;
    // TIM_TimeBaseInitStructure.TIM_ClockDivision=;//TIM6没有时钟分频
    // TIM_TimeBaseInitStructure.TIM_RepetitionCounter=;//只有TIM1和TIM8才有
    TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure);

    //NVIC初始化结构体定义
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel=TIM6_DAC_IRQn ; //定时器6中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3; //抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0; //子优先级0
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
 * 目前用于遥控
 **/
void TIM7_Init(void)
{
    //开TIM7时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_Prescaler=84-1;
    TIM_TimeBaseInitStructure.TIM_Period=1000-1;
    TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStructure);

    //定义NVIC初始化结构体
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel=TIM7_IRQn; //定时器5中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0; //子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

    //开TIM7中断
    TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);
    //TIM7使能
    TIM_Cmd(TIM7,ENABLE);
}