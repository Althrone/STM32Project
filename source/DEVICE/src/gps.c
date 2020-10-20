#include "gps.h"

#define BN_880  //北天通讯GNSS+罗盘模块

/**
 * @brief   GPS初始化，走USART4
 **/
void GPS_Init(void)
{
#ifdef BN_880
    sizeof(0xB56206090D000000);
    sizeof(uint64_t);
#endif /*BN_880*/
}

/**
 * @brief   GPS解码，将数据存储到对应结构体
 **/
void GPS_Decode(void)
{

}

/**
 * @brief   获取字符串头部
 **/
void GPS_GetHead(void)
{

}

/**
 * @brief   获取字符串结尾
 **/
void GPS_GetEnd(void)
{

}

/**
 * @brief   获取字符串分隔符，也就是逗号
 **/
void GPS_GetComma(void)
{

}