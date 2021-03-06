/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H
#define __USART_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  I2C Init structure definition  
  */

/* Exported functions --------------------------------------------------------*/ 

void USART1_Init(void);
void UART4_Init(void);

#ifdef __cplusplus
}
#endif

#endif /*__USART_H */