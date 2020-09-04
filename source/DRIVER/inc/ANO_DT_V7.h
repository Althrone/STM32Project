// #ifndef _DATA_TRANSFER_H
// #define	_DATA_TRANSFER_H

// #include "stm32f4xx.h"

// enum ANTO_ID{
// 	//数据校验帧
// 	SEND_ID_CHECK           = 0x00,
	
// 	//飞控相关信息类
// 	SEND_ID_SENSOR_IMU      = 0x01,
// 	SEND_ID_SENSOR_COMPASS  = 0x02,
// 	SEND_ID_EULER           = 0x03,
// 	SEND_ID_QUAD            = 0x04,
// 	SEND_ID_ALTITUDE        = 0x05,
// 	SEND_ID_DRONE_MODE      = 0x06,
// 	SEND_ID_DRONE_SPEED     = 0x07,
// 	SEND_ID_POS_BIAS        = 0x08,
// 	SEND_ID_WINDSPEED       = 0x09,
// 	SEND_ID_TARGET_POSE     = 0x0A,
// 	SEND_ID_TARGET_SPEED    = 0x0B,
// 	SEND_ID_GPS             = 0x0C,
// 	SEND_ID_VOL_AND_CUR     = 0x0D,
// 	SEND_ID_MODEL_MODE      = 0x0E,
// 	SEND_ID_RGB             = 0x0F,
	
// 	//飞控控制量输出类
// 	SEND_ID_CONTROL_PWM     = 0x20,
// 	SEND_ID_CONTROL_POSE    = 0x21,
	
// 	//飞控接收信息类
// 	RECEIVE_ID_SENSOR_GPS      = 0x30,
// 	RECEIVE_ID_RESERVE         = 0x31,
// 	RECEIVE_ID_SENSOR_POS      = 0x32,
// 	RECEIVE_ID_SENSOR_SPEED    = 0x33,
// 	RECEIVE_ID_SENSOR_DISTANCE = 0x34,
	
// 	//飞控接收控制指令类
// 	RECEIVE_ID_REMOTE    = 0x40,
// 	RECEIVE_ID_REALTIME  = 0x41,
	
// 	//用户自定义帧
// 	SEND_ID_USERDATA1       = 0xF1,
// 	SEND_ID_USERDATA2       = 0xF2,
// 	SEND_ID_USERDATA3       = 0xF3,
// 	SEND_ID_USERDATA4       = 0xF4,
// 	SEND_ID_USERDATA5       = 0xF5,
// 	SEND_ID_USERDATA6       = 0xF6,
// 	SEND_ID_USERDATA7       = 0xF7,
// 	SEND_ID_USERDATA8       = 0xF8,
// 	SEND_ID_USERDATA9       = 0xF9,
// 	SEND_ID_USERDATA10      = 0xFA,
// };

// extern u32 uSystickCount;    //外部时钟计数

// void vSendDataPolling(void);
// u8 vReceiveData(u8 data);

// #endif