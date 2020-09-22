/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RGBLED_H
#define __RGBLED_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/

typedef enum
{
    RGBLED_White    =0b000,
    RGBLED_Yellow   =0b001,
    RGBLED_Purple   =0b010,
    RGBLED_Red      =0b011,
    RGBLED_Cyan     =0b100,
    RGBLED_Green    =0b101,
    RGBLED_Blue     =0b110,
}RGBLED_ColourTypeDef;

typedef enum
{
    RGBLED_1sMode=0,
    RGBLED_p5sMode=1,
}RGBLED_ModeTypeDef;

/* Exported functions --------------------------------------------------------*/ 

void RGBLED_Init(void);
void RGBLED_ColourSet(uint8_t colour);
void RGBLED_OFF(void);
void RGBLED_White(void);


#ifdef __cplusplus
}
#endif

#endif /*__RGBLED_H */