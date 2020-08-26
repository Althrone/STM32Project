#include "i2c.h"

/**
 * 修改记录：
 * 位置：source\FWLIB\src\stm32f4xx_i2c.c 204行
 * 原内容：freqrange = (uint16_t)(pclk1 / 1000000);
 * 原因：快速模式需要修改I2CX_CR2的FREQ，这里我相当于设置成为10MHz。
 * 
 * 位置：source\FWLIB\src\stm32f4xx_i2c.c 245行
 * 原内容：result = (uint16_t)(pclk1 / (I2C_InitStruct->I2C_ClockSpeed * 25));
 * 原因：我觉得是硬件IIC少人用，参考手册误导了，这里程序也误导了，
 * 正确计算方法应该是用I2C_CR2的FREQ计算，就是说FREQ=10的话，计算
 * 频率就应该是10MHz而不是APB1的PCLK1=42MHz，因为这个是我之前验证
 * 过的。之前打寄存器的时候我IIC的时钟频率设为10MHZ，快速模式400KHz
 * 通信，计算应该是x=10M/（25*400K），详细推导请看参考手册RM0090
 * 的I2C_CCR计算部分，注意把TPCLK1换成FREQ的周期，I2C_TRISE的计算
 * 同理。
 * 
 * 吐槽：然后source\FWLIB\src\stm32f4xx_i2c.c 259行的最大上升时间
 * 设置又正确了，莫名奇妙。
 **/
void I2C1_Init(void)
{
    //开GPIOB时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
    //GPIO初始化结构体定义
    GPIO_InitTypeDef GPIO_InitStructure;
    //I2C1_SCL
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_I2C1);
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    //I2C1_SDA
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_I2C1);
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;
    GPIO_Init(GPIOB,&GPIO_InitStructure);

    //开I2C时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
    //I2C初始化结构体定义
    I2C_InitTypeDef I2C_InitStructure;
    I2C_InitStructure.I2C_ClockSpeed=400000;
    I2C_InitStructure.I2C_Mode=I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle=I2C_DutyCycle_16_9;
    I2C_InitStructure.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C1,&I2C_InitStructure);

}