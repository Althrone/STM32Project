#include "systick.h"

/* SysTick public functions --------------------------------------------------*/

/* SysTick private functions -------------------------------------------------*/

// void SysTick_Init(void)
// {
//     SysTick->CALIB=1<<
// }

/**
 * @brief  SysTick微秒级延时
 * @param  microsecond: 微秒
 *         只能是1~99864
 **/
void SysTick_DelayUs(uint32_t microsecond)
{
    uint32_t temp;
    microsecond=microsecond*168/8-1;
    SysTick->CTRL = 0x00; //失能SysTick定时器
    SysTick->LOAD = microsecond; //计算重装载值
    SysTick->VAL  = 0x00; //清空计数器
    SysTick->CTRL|= 0x01; //启动SysTick定时器
    do
    {
        temp=SysTick->CTRL;
    }
    while((temp&0x01)&&!(temp&(1<<16))); //循环等待延时结束
    SysTick->CTRL = 0x00; //失能SysTick定时器
    SysTick->VAL  = 0x00;//清空计数器(这个必须有否则将会死在while)
}

/**
 * @brief  SysTick毫秒级延时
 * @param  microsecond: 毫秒
 *         只能是1~99
 **/
void SysTick_DelayMs(uint32_t millisecond)
{
    uint32_t temp;
    millisecond=millisecond*168000/8-1;
    SysTick->CTRL = 0x00; //失能SysTick定时器
    SysTick->LOAD = millisecond; //计算重装载值
    SysTick->VAL  = 0x00; //清空计数器
    SysTick->CTRL|= 0x01; //启动SysTick定时器
    do
    {
        temp=SysTick->CTRL;
    }
    while((temp&0x01)&&!(temp&(1<<16))); //循环等待延时结束
    SysTick->CTRL = 0x00; //失能SysTick定时器
    SysTick->VAL  = 0x00;//清空计数器(这个必须有否则将会死在while)
}

/**
 * @brief  放在source\USER\stm32f4xx_it.c
 **/
// void SysTick_Handler(void)
// {

// }
