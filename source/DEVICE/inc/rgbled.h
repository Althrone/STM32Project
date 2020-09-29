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
    RGBLED_Black    =0b111,
}RGBLED_ColourTypeDef;

typedef enum
{
    RGBLED_p5sMode  =0,//0.5s快闪
    RGBLED_1sMode   =1,//1s闪烁
    RGBLED_LongMode =2,//常亮
}RGBLED_ModeTypeDef;

/* Exported functions --------------------------------------------------------*/ 

void RGBLED_Init(void);
void RGBLED_StateSet(RGBLED_ColourTypeDef RGBLED_ColourEnum,
                     RGBLED_ModeTypeDef RGBLED_ModeEnum);
// void RGBLED_OFF(void);
// void RGBLED_White(void);


#ifdef __cplusplus
}
#endif

#endif /*__RGBLED_H */