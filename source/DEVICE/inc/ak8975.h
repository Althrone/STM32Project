/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AK8975_H
#define __AK8975_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

#include "iic_moni.h"

/* Device Address ------------------------------------------------------------*/
#define AK8975_CAD1_LOW_CAD0_LOW    0x0C
#define AK8975_CAD1_LOW_CAD0_HIGH   0x0D
#define AK8975_CAD1_HIGH_CAD0_LOW   0x0E
#define AK8975_CAD1_HIGH_CAD0_HIGH  0x0F

/* Memory Map ----------------------------------------------------------------*/
#define AK8975_WIA      0x00
#define AK8975_INFO     0x01
#define AK8975_ST1      0x02
#define AK8975_HXL      0x03
#define AK8975_HXH      0x04
#define AK8975_HYL      0x05
#define AK8975_HYH      0x06
#define AK8975_HZL      0x07
#define AK8975_HZH      0x08
#define AK8975_ST2      0x09
#define AK8975_CNTL     0x0A
#define AK8975_RSV      0x0B    //不要访问
#define AK8975_ASTC     0x0C
#define AK8975_TS1      0x0D    //不要访问
#define AK8975_TS2      0x0E    //不要访问
#define AK8975_I2CDIS   0x0F
#define AK8975_ASAX     0x10    //一次性熔丝ROM，不建议烧写
#define AK8975_ASAY     0x11    //一次性熔丝ROM，不建议烧写
#define AK8975_ASAZ     0x12    //一次性熔丝ROM，不建议烧写

/* Register Bit --------------------------------------------------------------*/
#define AK8975_ST1_DRDY     (1<<0)

#define AK8975_ST2_DERR     (1<<2)
#define AK8975_ST2_HOFL     (1<<3)

#define AK8975_CNTL_MODE_SHIFT          (0)//没想到好名字，先将就用着吧，反正我只用第二个
#define AK8975_CNTL_MODE_MASK           (15<<AK8975_CNTL_MODE_SHIFT)
    #define AK8975_CNTL_MODE_POWERDOWN  (0<<AK8975_CNTL_MODE_SHIFT)
    #define AK8975_CNTL_MODE_MEAS       (1<<AK8975_CNTL_MODE_SHIFT)
    #define AK8975_CNTL_MODE_SELFTESE   (8<<AK8975_CNTL_MODE_SHIFT)
    #define AK8975_CNTL_MODE_FUSEACCESS (15<<AK8975_CNTL_MODE_SHIFT)
/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  I2C Init structure definition  
  */

typedef struct
{
    int8_t ASAX;
    int8_t ASAY;
    int8_t ASAZ;
}AK8975_PRMTypeDef;


typedef struct
{
    int16_t AK8975_RawMagX;
    int16_t AK8975_RawMagY;
    int16_t AK8975_RawMagZ;
}AK8975_RawDataTypeDef;

typedef struct
{
    float_t AK8975_FloatMagX;
    float_t AK8975_FloatMagY;
    float_t AK8975_FloatMagZ;
}AK8975_FloatDataTypeDef;

/* Exported functions --------------------------------------------------------*/ 

void AK8975_Init(void);
void AK8975_AllRawDataRead(AK8975_RawDataTypeDef* AK8975_RawDataStruct);
void AK8975_RawData2FloatData(AK8975_RawDataTypeDef* AK8975_RawDataStruct,
                              AK8975_FloatDataTypeDef* AK8975_FloatDataStruct);

void AK8975_IDRead(uint8_t* data);

#ifdef __cplusplus
}
#endif

#endif /*__AK8975_H */