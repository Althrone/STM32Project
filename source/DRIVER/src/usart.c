#include "usart.h"

/* USART public functions ----------------------------------------------------*/
/* USART private functions ---------------------------------------------------*/

/**
 * 添加记录：
 * 位置：source\FWLIB\inc\stm32f4xx_usart.h 175行
 * 新增内容：#define USART_Mode_Rx_Tx                     ((uint16_t)0x000C)
 * 原因：参考USART_HardwareFlowControl的
 * USART_HardwareFlowControl_RTS_CTS进行修改
 **/
void USART1_Init(void)
{
    //开GPIOA时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
    //GPIO初始化结构体定义
    GPIO_InitTypeDef GPIO_InitStructure;
    //USART1_TX
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    //USART1_RX
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    //开USART1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    //USART初始化结构体定义
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate=;
    USART_InitStructure.USART_WordLength=USART_WordLength_8b;
    USART_InitStructure.USART_StopBits=USART_StopBits_1;
    USART_InitStructure.USART_Parity=USART_Parity_No;
    USART_InitStructure.USART_Mode=USART_Mode_Rx_Tx;
    USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART_Init(USART1,&USART_InitStructure);
}