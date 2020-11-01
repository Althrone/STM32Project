#include "gps.h"

List_NodeTypeDef* List_HeadPointer=NULL;
static GPS_StateTypeDef gpsState;//参考GPS_StateTypeDef
GPS_DataTypeDef GPS_DataStruct;//GPS获取到的所有数据


#define BN_880      //北天通讯 GNSS+罗盘模块
// #define M8N         //pixhawk GPS+罗盘模块
// #define ATGM336H    //中科微电子 GPS

//只获取RMC

#define Km_per_h2m_per_h    (1000.f/3600.f)
#define m_per_h2Km_per_h    (3600.f/1000.f)

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

//初始化EXTI
}

/**
 * @brief   GPS解码，将数据存储到对应结构体
 **/
void GPS_Decode(void)
{
    //和校验检测
    if(GPS_CheckSum())
    {
        //创建临时指针指向链表头节点
        List_NodeTypeDef* tmpPointer=List_HeadPointer;
        //判断卫星种类 忽略 过两个有效节点
        tmpPointer=tmpPointer->List_NextNode->List_NextNode->List_NextNode;
        //判断内容种类
        uint32_t tmpVal=0;
        while(tmpPointer->List_NodeData!=',')
        {
            tmpVal<<8;
            tmpVal+=tmpPointer->List_NodeData;
            tmpPointer=tmpPointer->List_NextNode;
        }
        //跳出循环的时候指向','
        //根据内容种类和卫星种类将数据分配到不同结构体中，判断逗号作为分隔符
        switch (tmpVal)
        {
        case GPS_ContZDA:
            GPS_DecodeZDA(tmpPointer);
            break;
        case GPS_ContGGA:
            /* code */
            break;
        case GPS_ContGLL:
            /* code */
            break;
        case GPS_ContVTG:
            /* code */
            break;
        case GPS_ContGSA:
            /* code */
            break;
        case GPS_ContGSV:
            /* code */
            break;
        case GPS_ContRMC:
            GPS_DecodeRMC(tmpPointer);
            /* code */
            break;
        case GPS_ContANT:
            /* code */
            break;
        default:
            break;
        }
        //串口接收到的内容应该动态销毁
        //解码完成
        List_DeleteList(&List_HeadPointer);
        gpsState=GPS_StateFinish;
    }
    //和校验出错，销毁链表，GPS重新接收数据
    List_DeleteList(&List_HeadPointer);
    gpsState=GPS_StateFinish;
}

/**
 * @brief   信号和校验，采用异或计算，计算范围为$~*之间
 * @param   str: 字符串指针，相对地址，同时也作为返回值
 * @return  true表示和校验成功，false表示和校验失败
 **/
bool GPS_CheckSum(void)
{
    //临时链表指针
    List_NodeTypeDef* tmp;
    tmp=List_HeadPointer->List_NextNode;
    //异或校验
    uint8_t sum1=0;//计算异或和校验
    uint8_t sum2;//获取GPS发来的和校验值
    while(tmp->List_NodeData!='*')
    {
        sum1^=tmp->List_NodeData;
        tmp=tmp->List_NextNode;
    }
    //*号后一位
    tmp=tmp->List_NextNode;
    sum2=GPS_ASCII2Hex(tmp->List_NodeData)<<4;
    tmp=tmp->List_NextNode;
    sum2+=GPS_ASCII2Hex(tmp->List_NodeData);
    if(sum1==sum2)
        return true;
    else
        return false;
}

/**
 * @brief   解码ZDA序列，主要是UTC时间和日期
 **/
void GPS_DecodeZDA(List_NodeTypeDef* NodePointer)
{
    // NodePointer->List_NextNode;//跳过','
    // while(NodePointer->List_NodeData!=',')
    // {
        
    // }
}

/**
 * @brief   解码RMC序列，日期，时间，经纬度，对地速度，方位角，磁偏角
 **/
void GPS_DecodeRMC(List_NodeTypeDef* NodePointer)
{
    NodePointer=NodePointer->List_NextNode;//跳过','
    GPS_ASCII2Date(&NodePointer,&GPS_DataStruct.GPS_TimeStruct);
}

/**
 * @brief   字符串转十六进制
 **/
uint8_t GPS_ASCII2Hex(uint8_t input)
{
    if((input>='0')&&(input<='9'))//数字字符
        input-=0x30;
    if((input>='A')&&(input<='Z'))//大写字母字符
        input-=0x37;
    if((input>='a')&&(input<='z'))//小写字母字符
        input-=0x57;
    return input;
}

/**
 * @brief   ASCII转度，用于经纬度转换
 **/
float_t GPS_ASCII2Angel(List_NodeTypeDef** NodePointer)
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
float_t GPS_ASCII2Float(List_NodeTypeDef** NodePointer)
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
 * @brief   ASCII码转UTC时间
 * @param   NodePointer: 链表指针的指针，因为要修改指针的值
 * @param   GPS_TimeStruct: GPS获取时间结构体
 * @return  位移后的指针地址
 **/
void GPS_ASCII2Time(List_NodeTypeDef** NodePointer,GPS_TimeTypeDef* GPS_TimeStruct)
{
    
    // GPS_TimeStruct->GPS_Hour=*NodePointer
    //将字符串的数据转到
    // GPS_TimeStruct->GPS_Hour=*(str+0)*10+*(str+1);
    // GPS_TimeStruct->GPS_Minute=*(str+2)*10+*(str+3);
    // GPS_TimeStruct->GPS_Second=*(str+4)*10+*(str+5);
    // // *(str+6)='.'忽略
    // GPS_TimeStruct->GPS_Millisecond=*(str+7)*100+*(str+8)*10+*(str+9);
    // str+=10;
}

/**
 * @brief   ASCII码转日期
 * @param   str: 字符串的指针，相对地址
 * @param   GPS_TimeStruct: GPS获取时间结构体
 * @return  位移后的指针地址
 **/
void GPS_ASCII2Date(List_NodeTypeDef** NodePointer,GPS_TimeTypeDef* GPS_TimeStruct)
{
    //将字符串的数据转到
    // GPS_TimeStruct->GPS_Day=*(str+0)*10+*(str+1);
    // GPS_TimeStruct->GPS_Month=*(str+2)*10+*(str+3);
    // GPS_TimeStruct->GPS_Year=*(str+4)*10+*(str+5);
    // str+=6;
}

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
            {
                gpsState=GPS_StateStart;
                //创建链表
                List_CreatHead(&List_HeadPointer);
            }
            break;
        case 0x0D://可能是回车或者校验位
            gpsState=GPS_StatePrepEnd;
            break;
        case 0x0A://可能是换行或者校验位
            switch (gpsState)
            {
            case GPS_StateStart:
                //写入链表
                List_AddNode(List_HeadPointer,tmp);
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
                List_AddNode(List_HeadPointer,tmp);
                break;
            case GPS_StatePrepEnd://前一个值是0x0D
                //0x0D写入链表
                List_AddNode(List_HeadPointer,0x0D);
                //这次获取的值也写入链表
                List_AddNode(List_HeadPointer,tmp);
                gpsState=GPS_StateStart;
                break;
            default:
                break;
            }
            break;
        }
    //清除标志位
    USART_ClearFlag(UART4,USART_FLAG_RXNE);
    }
    
}

/**
 * @brief   外部中断0进行GPS链表解码
 **/
void EXTI0_IRQHandler(void)
{
    //先进行和校验
}