#include "steer.h"

/**
 * PWMOUT_5-TIM5_CH4-PA3
 * PWMOUT_6-TIM5_CH3-PA2
 **/
void Steer_Init(void)
{
    //开GPIOA时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
    //GPIO初始化结构体定义
    GPIO_InitTypeDef GPIO_InitStructure;
    //PWMOUT_5
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_TIM5);
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    //PWMOUT_6
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM5);
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    TIM5_Init();
}

void Steer_Test(ANO_DT_SendRCDataTypeDef* ANO_DT_SendRCDataStruct)
{
    uint16_t Throttle;
    if(ANO_DT_SendRCDataStruct->ANO_DT_RCThrottle>2000)
    Throttle=2000;
    else if(ANO_DT_SendRCDataStruct->ANO_DT_RCThrottle<1000)
    Throttle=1000;
    else
    Throttle=ANO_DT_SendRCDataStruct->ANO_DT_RCThrottle;

    TIM_SetCompare3(TIM5,Throttle*2);
    TIM_SetCompare4(TIM5,Throttle*2);
}
