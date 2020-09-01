#include "tim.h"

/* I2C public functions ------------------------------------------------------*/ 

/* I2C1 private functions ----------------------------------------------------*/ 

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
    TIM_TimeBaseInitStructure.TIM_Prescaler=4200;
    // TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//TIM6只有向上计数模式
    TIM_TimeBaseInitStructure.TIM_Period=0x2710;
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