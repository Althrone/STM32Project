/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AT24C02_H
#define __AT24C02_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "iic_moni.h"

#include "i2c.h"

/* Device Address ------------------------------------------------------------*/
#define AT24C02_A2_LOW_A1_LOW_A0_LOW    0x50
#define AT24C02_A2_LOW_A1_LOW_A0_HIGH   0x51
#define AT24C02_A2_LOW_A1_HIGH_A0_LOW   0x52
#define AT24C02_A2_LOW_A1_HIGH_A0_HIGH  0x53
#define AT24C02_A2_HIGH_A1_LOW_A0_LOW   0x54
#define AT24C02_A2_HIGH_A1_LOW_A0_HIGH  0x55
#define AT24C02_A2_HIGH_A1_HIGH_A0_LOW  0x56
#define AT24C02_A2_HIGH_A1_HIGH_A0_HIGH 0x57

/* Memory Map ----------------------------------------------------------------*/
#define AT24C02_HASH                    0x00//AT24C02整个表的哈希值

#define AT24C02_GORY_X_OFFSET           0x04//陀螺仪零漂，浮点数
#define AT24C02_GORY_Y_OFFSET           0x08
#define AT24C02_GORY_Z_OFFSET           0x0C

#define AT24C02_ACCEL_X_SCALE           0x10//加速度计刻度因子
#define AT24C02_ACCEL_Y_SCALE           0x18
#define AT24C02_ACCEL_Z_SCALE           0x20
#define AT24C02_ACCEL_X_BIAS            0x14//加速度计偏移
#define AT24C02_ACCEL_Y_BIAS            0x1C
#define AT24C02_ACCEL_Z_BIAS            0x24

#define AT24C02_MAG_X_SCALE             0x28//磁力计刻度因子
#define AT24C02_MAG_Y_SCALE             0x30
#define AT24C02_MAG_Z_SCALE             0x38
#define AT24C02_MAG_X_BIAS              0x2C//磁力计偏移
#define AT24C02_MAG_Y_BIAS              0x34
#define AT24C02_MAG_Z_BIAS              0x3C


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

#endif /*__AT24C02_H */