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