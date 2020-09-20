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
    TIM_SetCompare1(TIM3,2000);
    TIM_SetCompare2(TIM3,2000);
    TIM_SetCompare3(TIM3,2000);
    TIM_SetCompare4(TIM3,2000);
}

/**
 ↻  ↺
 →   ←
↑3↓ ↓1↑
 ←\ /→
   X
 ←/ \→
↓2↑ ↑4↓
 →   ←
 ↺  ↻
**/

void Motor_PWM(Motor_PWMTypeDef* Motor_PWMStruct)
{
    TIM_SetCompare1(TIM3,Motor_PWMStruct->PWM1*2);
    TIM_SetCompare2(TIM3,Motor_PWMStruct->PWM2*2);
    TIM_SetCompare3(TIM3,Motor_PWMStruct->PWM3*2);
    TIM_SetCompare4(TIM3,Motor_PWMStruct->PWM4*2);
}

/**
 * @brief   测试行走功能
 * @param   ANO_DT_SendRCDataStruct: 接收遥控的信号
 **/
void Motor_RunTest(ANO_DT_SendRCDataTypeDef* ANO_DT_SendRCDataStruct)
{
    // uint16_t Pitch,Roll;
    // //俯仰控制前进后退
    // if(ANO_DT_SendRCDataStruct->ANO_DT_RCPitch>2000)
    // Pitch=2000;
    // else if(ANO_DT_SendRCDataStruct->ANO_DT_RCPitch<1000)
    // Pitch=1000;
    // else
    // Pitch=ANO_DT_SendRCDataStruct->ANO_DT_RCPitch;

    // if(ANO_DT_SendRCDataStruct->ANO_DT_RCRoll>2000)
    // Roll=2000;
    // else if(ANO_DT_SendRCDataStruct->ANO_DT_RCRoll<1000)
    // Roll=1000;
    // else
    // Roll=ANO_DT_SendRCDataStruct->ANO_DT_RCRoll;



    //横滚控制差速转弯
}