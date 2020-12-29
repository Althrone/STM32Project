/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPS_H
#define __GPS_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

#include <stdbool.h>

#include "usart.h"

// #include "usart"
/* Exported types ------------------------------------------------------------*/


/* Exported functions --------------------------------------------------------*/

void GPS_Init(void);

/* Private functions ---------------------------------------------------------*/
//不建议此文件外调用

uint16_t GPS_Checksum(uint8_t *buf,uint8_t length);
void GPS_CommandSend(USART_TypeDef* USARTx,uint8_t* GPS_Command,uint8_t length);
void GPS_BaudSet(USART_TypeDef* USARTx,uint32_t baud);


#ifdef __cplusplus
}
#endif

#endif /*__GPS_H */