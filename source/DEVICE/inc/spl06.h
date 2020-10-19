/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPL06_H
#define __SPL06_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

#include "iic_moni.h"

/* Device Address ------------------------------------------------------------*/
#define SPL06_AD0_LOW             0x76
#define SPL06_AD0_HIGH            0x77

/* Memory Map ----------------------------------------------------------------*/
#define SPL06_PSR_B2    0x00
#define SPL06_PSR_B1    0x01
#define SPL06_PSR_B0    0x02

#define SPL06_TMP_B2    0x03
#define SPL06_TMP_B1    0x04
#define SPL06_TMP_B0    0x05

#define SPL06_PRS_CFG   0x06
#define SPL06_TMP_CFG   0x07
#define SPL06_MEAS_CFG  0x08
#define SPL06_CFG_REG   0x09
#define SPL06_INT_STS   0x0A
#define SPL06_FIFO_STS  0x0B
#define SPL06_RESET     0x0C
#define SPL06_ID        0x0D

#define SPL06_COEF      0x10//0x10到0x21共18个字节
#define SPL06_COEF_C0H  0x10//c0的高8位
#define SPL06_COEF_C00H 0x13

/* Register Bit --------------------------------------------------------------*/
#define SPL06_PRS_CFG_PM_PRC_SHIFT      (0)
#define SPL06_PRS_CFG_PM_PRC_MASK       (15<<SPL06_PRS_CFG_PM_PRC_SHIFT)
    #define SPL06_PRS_CFG_PM_PRC_1      (0<<SPL06_PRS_CFG_PM_PRC_SHIFT)
    #define SPL06_PRS_CFG_PM_PRC_2      (1<<SPL06_PRS_CFG_PM_PRC_SHIFT)
    #define SPL06_PRS_CFG_PM_PRC_4      (2<<SPL06_PRS_CFG_PM_PRC_SHIFT)
    #define SPL06_PRS_CFG_PM_PRC_8      (3<<SPL06_PRS_CFG_PM_PRC_SHIFT)
    #define SPL06_PRS_CFG_PM_PRC_16     (4<<SPL06_PRS_CFG_PM_PRC_SHIFT)
    #define SPL06_PRS_CFG_PM_PRC_32     (5<<SPL06_PRS_CFG_PM_PRC_SHIFT)
    #define SPL06_PRS_CFG_PM_PRC_64     (6<<SPL06_PRS_CFG_PM_PRC_SHIFT)
    #define SPL06_PRS_CFG_PM_PRC_128    (7<<SPL06_PRS_CFG_PM_PRC_SHIFT)
#define SPL06_PRS_CFG_PM_RATE_SHIFT     (4)
#define SPL06_PRS_CFG_PM_RATE_MASK      (7<<SPL06_PRS_CFG_PM_RATE_SHIFT)
    #define SPL06_PRS_CFG_PM_RATE_1     (0<<SPL06_PRS_CFG_PM_RATE_SHIFT)
    #define SPL06_PRS_CFG_PM_RATE_2     (1<<SPL06_PRS_CFG_PM_RATE_SHIFT)
    #define SPL06_PRS_CFG_PM_RATE_4     (2<<SPL06_PRS_CFG_PM_RATE_SHIFT)
    #define SPL06_PRS_CFG_PM_RATE_8     (3<<SPL06_PRS_CFG_PM_RATE_SHIFT)
    #define SPL06_PRS_CFG_PM_RATE_16    (4<<SPL06_PRS_CFG_PM_RATE_SHIFT)
    #define SPL06_PRS_CFG_PM_RATE_32    (5<<SPL06_PRS_CFG_PM_RATE_SHIFT)
    #define SPL06_PRS_CFG_PM_RATE_64    (6<<SPL06_PRS_CFG_PM_RATE_SHIFT)
    #define SPL06_PRS_CFG_PM_RATE_128   (7<<SPL06_PRS_CFG_PM_RATE_SHIFT)

#define SPL06_TMP_CFG_TMP_PRC_SHIFT     (0)
#define SPL06_TMP_CFG_TMP_PRC_MASK      (7<<SPL06_TMP_CFG_TMP_PRC_SHIFT)
    #define SPL06_TMP_CFG_TMP_PRC_1     (0<<SPL06_TMP_CFG_TMP_PRC_SHIFT)
    #define SPL06_TMP_CFG_TMP_PRC_2     (1<<SPL06_TMP_CFG_TMP_PRC_SHIFT)
    #define SPL06_TMP_CFG_TMP_PRC_4     (2<<SPL06_TMP_CFG_TMP_PRC_SHIFT)
    #define SPL06_TMP_CFG_TMP_PRC_8     (3<<SPL06_TMP_CFG_TMP_PRC_SHIFT)
    #define SPL06_TMP_CFG_TMP_PRC_16    (4<<SPL06_TMP_CFG_TMP_PRC_SHIFT)
    #define SPL06_TMP_CFG_TMP_PRC_32    (5<<SPL06_TMP_CFG_TMP_PRC_SHIFT)
    #define SPL06_TMP_CFG_TMP_PRC_64    (6<<SPL06_TMP_CFG_TMP_PRC_SHIFT)
    #define SPL06_TMP_CFG_TMP_PRC_128   (7<<SPL06_TMP_CFG_TMP_PRC_SHIFT)
#define SPL06_TMP_CFG_TMP_RATE_SHIFT    (4)
#define SPL06_TMP_CFG_TMP_RATE_MASK     (7<<SPL06_TMP_CFG_TMP_RATE_SHIFT)
    #define SPL06_TMP_CFG_TMP_RATE_1    (0<<SPL06_TMP_CFG_TMP_RATE_SHIFT)
    #define SPL06_TMP_CFG_TMP_RATE_2    (1<<SPL06_TMP_CFG_TMP_RATE_SHIFT)
    #define SPL06_TMP_CFG_TMP_RATE_4    (2<<SPL06_TMP_CFG_TMP_RATE_SHIFT)
    #define SPL06_TMP_CFG_TMP_RATE_8    (3<<SPL06_TMP_CFG_TMP_RATE_SHIFT)
    #define SPL06_TMP_CFG_TMP_RATE_16   (4<<SPL06_TMP_CFG_TMP_RATE_SHIFT)
    #define SPL06_TMP_CFG_TMP_RATE_32   (5<<SPL06_TMP_CFG_TMP_RATE_SHIFT)
    #define SPL06_TMP_CFG_TMP_RATE_64   (6<<SPL06_TMP_CFG_TMP_RATE_SHIFT)
    #define SPL06_TMP_CFG_TMP_RATE_128  (7<<SPL06_TMP_CFG_TMP_RATE_SHIFT)
#define SPL06_TMP_CFG_TMP_EX            (1<<7)

#define SPL06_MEAS_CFG_MEAS_CTRL_SHIFT          (0)
#define SPL06_MEAS_CFG_MEAS_CTRL_MASK           (7<<SPL06_MEAS_CFG_MEAS_CTRL_SHIFT)
    #define SPL06_MEAS_CFG_MEAS_CTRL_SBY        (0<<SPL06_MEAS_CFG_MEAS_CTRL_SHIFT)
    #define SPL06_MEAS_CFG_MEAS_CTRL_CMDPRS     (1<<SPL06_MEAS_CFG_MEAS_CTRL_SHIFT)
    #define SPL06_MEAS_CFG_MEAS_CTRL_CMDTMP     (2<<SPL06_MEAS_CFG_MEAS_CTRL_SHIFT)
    #define SPL06_MEAS_CFG_MEAS_CTRL_BGPRS      (5<<SPL06_MEAS_CFG_MEAS_CTRL_SHIFT)
    #define SPL06_MEAS_CFG_MEAS_CTRL_BGTMP      (6<<SPL06_MEAS_CFG_MEAS_CTRL_SHIFT)
    #define SPL06_MEAS_CFG_MEAS_CTRL_BGPRSTMP   (7<<SPL06_MEAS_CFG_MEAS_CTRL_SHIFT)
#define SPL06_MEAS_CFG_PRS_RDY                  (1<<4)
#define SPL06_MEAS_CFG_TMP_RDY                  (1<<5)
#define SPL06_MEAS_CFG_SENSOR_RDY               (1<<6)
#define SPL06_MEAS_CFG_COEF_RDY                 (1<<7)

#define SPL06_CFG_REG_FIFO_EN                   (1<<1)
#define SPL06_CFG_REG_P_SHIFT                   (1<<2)
#define SPL06_CFG_REG_T_SHIFT                   (1<<3)
#define SPL06_CFG_REG_INT_TMP                   (1<<4)
#define SPL06_CFG_REG_INT_PRS                   (1<<5)
#define SPL06_CFG_REG_INT_FIFO                  (1<<6)
#define SPL06_CFG_REG_INT_HL                    (1<<7)

#define SPL06_INT_STS_INT_PRS                   (1<<0)
#define SPL06_INT_STS_INT_TMP                   (1<<1)
#define SPL06_INT_STS_INT_FIFO_FULL             (1<<2)

#define SPL06_FIFO_STS_FIFO_EMPTY               (1<<0)
#define SPL06_FIFO_STS_FIFO_FULL                (1<<1)

#define SPL06_RESET_SOFT_RST                    (0x09<<0)
#define SPL06_RESET_FIFO_FLUSH                  (1<<7)

#define SPL06_ID_REV_ID                         ()
#define SPL06_ID_PROD_ID                        ()

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  I2C Init structure definition  
  */

typedef enum
{
    SPL06_Single    =524288,
    SPL06_2Times    =1572864,
    SPL06_4Times    =3670016,
    SPL06_8Times    =7864320,
    SPL06_16Times   =253952,
    SPL06_32Times   =516096,
    SPL06_64Times   =1040384,
    SPL06_128Times  =2088960,
}SPL06_CompScaleFactorsTypeDef;

typedef struct
{
    int32_t SPL06_RawPres;
    int32_t SPL06_RawTemp;
}SPL06_RawDataTypeDef;

typedef struct
{
    float_t SPL06_FloatPres;
    float_t SPL06_FloatTemp;
}SPL06_FloatDataTypeDef;


typedef struct
{
    int16_t c0;//用于计算温度补偿值
    int16_t c1;
    int32_t c00;//计算压力补偿值
    int32_t c10;
    int16_t c01;
    int16_t c11;
    int16_t c20;
    int16_t c21;
    int16_t c30;
}SPL06_PRMTypeDef;


/* Exported functions --------------------------------------------------------*/ 
void SPL06_Init(void);
void SPL06_AllRawDataRead(SPL06_RawDataTypeDef* SPL06_RawDataStruct);
void SPL06_RawData2FloatData(SPL06_RawDataTypeDef* SPL06_RawDataStruct,
                             SPL06_FloatDataTypeDef* SPL06_FloatDataStruct);
void SPL06_RawData2Altitude(SPL06_RawDataTypeDef* SPL06_RawDataStruct,float_t* alt);
void SPL06_IDRead(uint8_t* data);



#ifdef __cplusplus
}
#endif

#endif /*__SPL06_H */