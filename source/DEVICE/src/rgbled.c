#include "rgbled.h"

void RGBLED_Init(void)
{
    //开GPIOE时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
    //GPIO初始化结构体定义
    GPIO_InitTypeDef GPIO_InitStructure;
    //Red
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_Init(GPIOE,&GPIO_InitStructure);
    //Green
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_Init(GPIOE,&GPIO_InitStructure);
    //Blue
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_Init(GPIOE,&GPIO_InitStructure);
}

/**
 * @brief   设定RGB的颜色和闪烁频率
 * @param   RGBLED_ColourEnum: 指定led颜色
 * @param   mode: 闪烁模式
 **/
void RGBLED_StateSet(RGBLED_ColourTypeDef RGBLED_ColourEnum,
                     RGBLED_ModeTypeDef RGBLED_ModeEnum)
{
    if ((RGBLED_ColourEnum&0b001)==0b001)
        GPIO_SetBits(GPIOE,GPIO_Pin_0);
    else
        GPIO_ResetBits(GPIOE,GPIO_Pin_0);
    if ((RGBLED_ColourEnum&0b010)==0b010)
        GPIO_SetBits(GPIOE,GPIO_Pin_1);
    else
        GPIO_ResetBits(GPIOE,GPIO_Pin_1);
    if ((RGBLED_ColourEnum&0b100)==0b100)
        GPIO_SetBits(GPIOE,GPIO_Pin_2);
    else
        GPIO_ResetBits(GPIOE,GPIO_Pin_2);
}

// void RGBLED_OFF(void)
// {
//     GPIO_SetBits(GPIOE,GPIO_Pin_0);
//     GPIO_SetBits(GPIOE,GPIO_Pin_1);
//     GPIO_SetBits(GPIOE,GPIO_Pin_2);
// }

// void RGBLED_White(void)
// {
//     GPIO_ResetBits(GPIOE,GPIO_Pin_0);
//     GPIO_ResetBits(GPIOE,GPIO_Pin_1);
//     GPIO_ResetBits(GPIOE,GPIO_Pin_2);
// }

