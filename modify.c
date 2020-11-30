/**
 * 修改记录：
 * 位置：source\USER\system_stm32f4xx.c 371行
 * 原内容：#define PLL_M      25
 * 新内容：#define PLL_M      8
 * 原因：做了个定时器跑1s，结果发现很慢，差不多3s，就来检察时钟。
 **/

/**
 * 修改记录：
 * 位置：source\USER\stm32f4xx.h 144行
 * 原内容：#define HSE_VALUE    ((uint32_t)25000000)
 * 新内容：#define HSE_VALUE    ((uint32_t)8000000)
 * 原因：串口误码率很高，然后感觉是系统时钟的问题，于是就检查一下，发现是和之前改过的PLL_M有关系，选不同的晶振这两个值都要一起改的
 **/

/**
 * 修改记录：
 * 位置：source\FWLIB\src\stm32f4xx_i2c.c 204行
 * 原内容：freqrange = (uint16_t)(pclk1 / 1000000);
 * 新内容：freqrange = (uint16_t)(pclk1 / 4200000);
 * 然后我又改回去了，原因是I2C_CR2的FREQ实际上是用来指示PLCK1
 * 的时钟频率的，并没有分频的作用。
 * 
 * 位置：source\FWLIB\src\stm32f4xx_i2c.c 245行
 * 原内容：result = (uint16_t)(pclk1 / (I2C_InitStruct->I2C_ClockSpeed * 25));
 * 新内容：result = (uint16_t)(freqrange / (I2C_InitStruct->I2C_ClockSpeed * 25));
 * 我也改回去了，原来的并没有错。
 * 
 * 然后有一个新问题，我希望iic总线跑在400KHz的频率，但是如果配置成
 * 16/9模式，CCR计算结果是4.2，向下取整速度就比400KHz快了，
 * 假设CCR取5，实际的通信速度是336KHz，也不符合要求。然后我尝试占
 * 空比用2/1模式，发现CCR=35，这个值可取，但是手册要求想达到
 * 400KHz，PCLK1频率必须为10MHz的倍数才能实现。
 **/