#include "usart.h"

/* USART public functions ----------------------------------------------------*/
/* USART private functions ---------------------------------------------------*/

/**
 * 添加记录：
 * 位置：source\FWLIB\inc\stm32f4xx_usart.h 175行
 * 新增内容：#define USART_Mode_Rx_Tx                     ((uint16_t)0x000C)
 * 原因：参考USART_HardwareFlowControl的
 * USART_HardwareFlowControl_RTS_CTS进行修改
 * 这个寄存器准备用于与上位机通信，只负责发送
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
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    //USART1_RX
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;//GPIO_PuPd_UP
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    //开USART1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    //USART初始化结构体定义
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate=115200;//460800
    USART_InitStructure.USART_WordLength=USART_WordLength_8b;
    USART_InitStructure.USART_StopBits=USART_StopBits_1;
    USART_InitStructure.USART_Parity=USART_Parity_No;
    USART_InitStructure.USART_Mode=USART_Mode_Rx_Tx;
    USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART_Init(USART1,&USART_InitStructure);

    // //NVI初始化结构体定义
    // NVIC_InitTypeDef NVIC_InitStructure;
    // NVIC_InitStructure.NVIC_IRQChannel=USART6_IRQn ; //USART6中断
    // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
    // NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
    // NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    // NVIC_Init(&NVIC_InitStructure);

    //开USART1中断
    // USART_ITConfig(USART1,USART_IT_TXE,ENABLE);
    //USART1使能
    USART_Cmd(USART1,ENABLE);
}

/**
 * @brief   UART4初始化，用于GPS
 **/
void UART4_Init(void)
{
    //开GPIOA时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
    //GPIO初始化结构体定义
    GPIO_InitTypeDef GPIO_InitStructure;
    //UART4_TX
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_UART4);
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    //UART4_RX
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_UART4);
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    //开UART4时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
    //USART初始化结构体定义
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate=9600;//9600
    USART_InitStructure.USART_WordLength=USART_WordLength_8b;
    USART_InitStructure.USART_StopBits=USART_StopBits_1;
    USART_InitStructure.USART_Parity=USART_Parity_No;
    USART_InitStructure.USART_Mode=USART_Mode_Rx_Tx;
    USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART_Init(UART4,&USART_InitStructure);

    //NVI初始化结构体定义
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel=UART4_IRQn; //UART4中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3; //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=3; //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //开USART1中断
    USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);
    USART_Cmd(UART4,ENABLE);//使能UART4
}

void USART1_IRQHandler(void)
{

}