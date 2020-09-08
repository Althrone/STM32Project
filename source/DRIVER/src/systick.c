#include "systick.h"

/* SysTick public functions --------------------------------------------------*/

/* SysTick private functions -------------------------------------------------*/

/**
 * @brief  SysTick初始化
 * @param  millisecond: 毫秒
 *         只能是1~99
 **/
void SysTick_Delay(uint32_t millisecond)
{
    millisecond=millisecond*168000000/1000;
    SysTick_Config(millisecond);
}
