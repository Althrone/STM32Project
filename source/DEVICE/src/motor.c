#include "motor.h"

/**
 * PWMOUT_1-TIM3_CH4-PB1
 * PWMOUT_2-TIM3_CH3-PB0
 * PWMOUT_3-TIM3_CH2-PA7
 * PWMOUT_4-TIM3_CH1-PA6
 **/
void Motor_Init(void)
{
    //开GPIOA，GPIOB时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
    //GPIO初始化结构体定义
    GPIO_InitTypeDef GPIO_InitStructure;
    //PWMOUT_1
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource1,GPIO_AF_TIM3);
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    //PWMOUT_2
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM3);
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    //PWMOUT_3
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3);
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    //PWMOUT_4
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3);
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    TIM3_Init();
}

void Motor_SetSpeed(void)
{
    TIM_SetCompare1(TIM3,4000);
    TIM_SetCompare2(TIM3,3000);
    TIM_SetCompare3(TIM3,3000);
    TIM_SetCompare4(TIM3,3000);
}