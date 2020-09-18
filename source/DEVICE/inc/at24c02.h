/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AHRS_H
#define __AHRS_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "iic_moni.h"

/* Device Address ------------------------------------------------------------*/
#define AT24C02_A2_LOW_A1_LOW_A0_LOW    0x50
#define AT24C02_A2_LOW_A1_LOW_A0_HIGH   0x51
#define AT24C02_A2_LOW_A1_HIGH_A0_LOW   0x52
#define AT24C02_A2_LOW_A1_HIGH_A0_HIGH  0x53
#define AT24C02_A2_HIGH_A1_LOW_A0_LOW   0x54
#define AT24C02_A2_HIGH_A1_LOW_A0_HIGH  0x55
#define AT24C02_A2_HIGH_A1_HIGH_A0_LOW  0x56
#define AT24C02_A2_HIGH_A1_HIGH_A0_HIGH 0x57

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  I2C Init structure definition  
  */



/* Exported functions --------------------------------------------------------*/ 
void AT24C02_ByteWrite(uint8_t addr,uint8_t data);
void AT24C02_PageWrite(uint8_t addr,uint8_t* data);
void AT24C02_CurrentAddressRead(uint8_t* buf);
void AT24C02_RandomRead(uint8_t* buf);
void AT24C02_SequentialRead(uint8_t addr,uint8_t length,uint8_t* data);


#ifdef __cplusplus
}
#endif

#endif /*__AHRS_H */