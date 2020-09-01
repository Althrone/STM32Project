/**
 * 修改记录：
 * 位置：source\USER\system_stm32f4xx.c 371行
 * 原内容：#define PLL_M      25
 * 新内容：#define PLL_M      8
 * 原因：做了个定时器跑1s，结果发现很慢，差不多3s，就来检察时钟。
 **/