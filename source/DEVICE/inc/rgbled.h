/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RGBLED_H
#define __RGBLED_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/ 

void RGBLED_Init(void);
void RGBLED_ColourSet(uint8_t colour);
void RGBLED_OFF(void);
void RGBLED_White(void);


#ifdef __cplusplus
}
#endif

#endif /*__RGBLED_H */