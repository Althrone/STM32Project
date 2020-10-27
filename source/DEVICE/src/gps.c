#include "gps.h"

#define BN_880      //北天通讯 GNSS+罗盘模块
// #define M8N         //pixhawk GPS+罗盘模块
// #define ATGM336H    //中科微电子 GPS

//只获取RMC

#define Km_per_h2m_per_h    (1000.f/3600.f)
#define m_per_h2Km_per_h    (3600.f/1000.f)

void GPS_Test(int* a)
{
    *a=GPS_ContZDA;
}

/**
 * @brief   GPS初始化，走USART4
 **/
void GPS_Init(void)
{
#if defined(BN_880)||defined(M8N)//ublox芯片

/* BN_880 || M8N */
#elif defined(ATGM336H)//中科微电子芯片

/* ATGM336H */
#endif 

}

/**
 * @brief   GPS解码，将数据存储到对应结构体
 **/
void GPS_Decode(void)
{
    //判断卫星种类
    //判断内容种类
    //根据内容种类和卫星种类将数据分配到不同结构体中，判断逗号作为分隔符
    //串口接收到的内容应该动态销毁
    //判断结尾
}

/**
 * @brief   获取字符串分隔符，也就是逗号
 **/
void GPS_GetComma(void)
{
    //
}

/**
 * @brief   信号和校验，采用异或计算，计算范围为$~*之间
 * @param   str: 字符串指针，相对地址，同时也作为返回值
 * @return  和校验的值sum
 **/
uint16_t GPS_CheckSum(uint8_t* str)
{
    //异或校验
    uint16_t sum=0;
    while(*str!='*')
    {
        str++;//
        sum^=*str;
    }
    return sum;
}

/**
 * @brief   ASCII转度，用于经纬度转换
 **/
void GPS_ASCII2Angel(uint8_t* str,float_t* value)
{
    float_t tmp1,tmp2;
    GPS_ASCII2Float(str,value);
    //求整
    tmp1=(int16_t)*value/100;
    //求余
    tmp2=fmodf(*value,100)/60;
    *value=tmp1+tmp2;
}

/**
 * @brief   ASCII码转浮点数
 * @param   str: 字符串的指针
 * @return  float: 浮点数
 **/
void GPS_ASCII2Float(uint8_t* str,float_t* value)
{
    uint8_t tmp,flag=0;
    float_t decimal;
    *value=0;
    decimal=0;
    while(((*str>='0')&&(*str<='9'))||(*str=='.')) //数字或者是符号
    {
        tmp=*str++;//先取值，然后地址自加
        if(*str=='.')
        {
            flag=1;
            tmp=*str++;
        }
        if(flag==0)//整数部分
            *value=*value*10.0f+(float_t)tmp;
        else if(flag==1)//小数部分
            decimal=decimal*10.0f+(float_t)tmp;
    }
    while (decimal>1)
        decimal=decimal/10.0f;
    *value+=decimal;
}

/**
 * @brief   ASCII码转时间
 * @param   str: 字符串的指针，相对地址
 * @param   GPS_TimeStruct: GPS获取时间结构体
 * @return  位移后的指针地址
 **/
void GPS_ASCII2Time(uint8_t* str,GPS_TimeTypeDef* GPS_TimeStruct)
{
    //将字符串的数据转到
    GPS_TimeStruct->GPS_Hour=*(str+0)*10+*(str+1);
    GPS_TimeStruct->GPS_Minute=*(str+2)*10+*(str+3);
    GPS_TimeStruct->GPS_Second=*(str+4)*10+*(str+5);
    // *(str+6)='.'忽略
    GPS_TimeStruct->GPS_Millisecond=*(str+7)*100+*(str+8)*10+*(str+9);
    str+=10;
}

/**
 * @brief   ASCII码转日期
 * @param   str: 字符串的指针，相对地址
 * @param   GPS_TimeStruct: GPS获取时间结构体
 * @return  位移后的指针地址
 **/
void GPS_ASCII2Date(uint8_t* str,GPS_TimeTypeDef* GPS_TimeStruct)
{
    //将字符串的数据转到
    GPS_TimeStruct->GPS_Day=*(str+0)*10+*(str+1);
    GPS_TimeStruct->GPS_Month=*(str+2)*10+*(str+3);
    GPS_TimeStruct->GPS_Year=*(str+4)*10+*(str+5);
    str+=6;
}

static uint8_t gpsState;//参考GPS_StateTypeDef

/**
 * @brief   UART4中断处理函数，用于GPS数据接收
 **/
void UART4_IRQHandler(void)
{
    if(USART_GetFlagStatus(UART4,USART_FLAG_RXNE)!=RESET)//接收到数据，进入中断
    {
        uint8_t tmp;
        tmp=USART_ReceiveData(UART4);
        switch (tmp)
        {
        case '$'://起始序列符号
            if(gpsState==GPS_StateFinish)//前一次的数据流解码已经完成
                gpsState=GPS_StateStart;
            break;
        case 0x0D://可能是回车或者校验位
            gpsState=GPS_StatePrepEnd;
            break;
        case 0x0A://可能是换行或者校验位
            switch (gpsState)
            {
            case GPS_StateStart:
                //写入链表
                break;
            case GPS_StatePrepEnd://前一个值是0x0D
                gpsState=GPS_StateEnd;
                //触发中断
            default:
                break;
            }
            break;
        default:
            switch (gpsState)
            {
            case GPS_StateStart:
                //写入链表
                break;
            case GPS_StatePrepEnd://前一个值是0x0D
                //0x0D写入链表
                //这次获取的值也写入链表
                gpsState=GPS_StateStart;
            default:
                break;
            }
            break;
        }
    }
    
}