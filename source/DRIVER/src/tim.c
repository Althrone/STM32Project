#include "tim.h"

/* I2C public functions ------------------------------------------------------*/ 

/* I2C1 private functions ----------------------------------------------------*/ 

/**
 * 试试定时1s
 * 
 **/
void TIM6_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_Prescaler=0x20D0;
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period=0xFFFF-0x2710;
    // TIM_TimeBaseInitStructure.TIM_ClockDivision=;//TIM6没有时钟分频
    // TIM_TimeBaseInitStructure.TIM_RepetitionCounter=;//只有TIM1和TIM8才有
    TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure);

    //开TIM6中断
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM6,ENABLE);

}

uint8_t flag=0;

void TIM6_DAC_IRQHandler(void)
{
    if(TIM_GetFlagStatus(TIM6,TIM_FLAG_Update)!=RESET)
    {
        TIM_ClearFlag(TIM6,TIM_FLAG_Update);
        flag=~flag;
        if(flag==0)
        {
            RGBLED_OFF();
        }
        else
            RGBLED_White();

    }
}