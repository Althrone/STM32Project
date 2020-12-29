#include "gps.h"

/* Pubilc variables ----------------------------------------------------------*/
//外部调用的全局变量

/* Private define ------------------------------------------------------------*/
#define GPS_BufLength 256

/* Private variables ---------------------------------------------------------*/
//本函数内使用的全局变量
static uint8_t GPS_DataBuf[GPS_BufLength]={0};    //GPS接收到的数据存储

/* Private variables GPS command list-----------------------------------------*/

//恢复默认设置
//-------------------|SYNC CHAR|-cls|-msg|--LENGTH-|-BbrMask-|-rs-|-RSV|CK_A CK_B|
uint8_t RESET[]     ={0xB5,0x62,0x06,0x04,0x04,0x00,0xFF,0x87,0x01,0x00};

//打开报文
uint8_t OPEN_[]={0xB5,0x62,};

//关闭报文
uint8_t CLOSE_[]={0xB5,0x62,};

//设置输出频率
uint8_t RATE_SET[]  ={};

//设置波特率
//-------------------|SYNC CHAR|-cls|-msg|--LENGTH-|-ID-|-RSV|-txReady-|--------mode-------|------baudRate-----|--iMask--|--oMask--|--flags--|---RSV---|CK_A CK_B|
uint8_t BAUD_SET[]  ={0xB5,0x62,0x06,0x00,0x14,0x00,0x01,0x00,0x00,0x00,0xC0,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x00,0x03,0x00,0x00,0x00,0x00,0x00};//无需在这里更改波特率
//mode是字节长8位数据，没校验，1停止位


//清除，保存和加载设置
//-------------------|SYNC CHAR|-cls|-msg|--LENGTH-|-----clearMask-----|------saveMask-----|------loadMask-----|deviceMask|CK_A CK_B|
uint8_t CLEAR_CFG[] ={0xB5,0x62,0x06,0x09,0x0D,0x00,0x1F,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x17};//最后两位通过计算得出
uint8_t SAVE_CFG[]  ={0xB5,0x62,0x06,0x09,0x0D,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x17};//31 BF
uint8_t LOAD_CFG[]  ={0xB5,0x62,0x06,0x09,0x0D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x1F,0x00,0x00,0x17};
u8 GPS_RESET[21]    ={0xB5,0x62,0x06,0x09,0x0D,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x03,0x1B,0x9A};//师兄的代码 满头问号


//思路：接收到的都放在 GPS_DataBuf 内，等接受完一次了就用dma传输到外部调用的全局变量

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Pubilc functions ----------------------------------------------------------*/

/**
 * @brief   GPS初始化，自动保存，掉电不会丢失，看看如何检测吧
 * @param   GPS_Command: 参见上面的GPS命令表
 **/
void GPS_Init(void)
{
    UART4_Init();
    //本函数内的宏设定，UART4作为和GPS通信的接口
    USART_TypeDef* USART_Struct=UART4;

    //波特率设置
    GPS_BaudSet(USART_Struct,921600);

    //保存设置
    // GPS_CommandSend(USART_Struct,SAVE_CFG,sizeof(SAVE_CFG));
}

/* Private functions ---------------------------------------------------------*/
//不建议此文件外调用

/**
 * @brief   接收/发送的数据进行校验
 * @param   buf: 接收情况下是 GPS_DataBuf[] ，发送情况下是各种GPS命令
 * @param   length: GPS命令/数据的长度，输入形式是sizeof(GPS_Command)/GPS_BufLength
 * @return  16位无符号数，因为大小端处理问题，顺序是 CK_B CK_A
 * @note    参考u-blox8-M8_ReceiverDescrProtSpec_(UBX-13003221) P171-P172
 *          这是一个弗莱彻校验算法(8-Bit Fletcher Algorithm)，
 *          能避免和校验对字节块调换不敏感的缺陷。从第三位开始计算，
 *          无论接收还是发送，最后两位校验都通过这个函数算出来。
 **/
uint16_t GPS_Checksum(uint8_t *buf,uint8_t length)
{
    uint8_t CK_A = 0;
    uint8_t CK_B = 0;
    for(uint8_t i=2;i<length;i++)
    {
        CK_A = CK_A + buf[i];
        CK_B = CK_B + CK_A;
        //CK_A和CK_B等于串口数据的最后两项的情况下就正确
    }
    return ((uint16_t)CK_B<<8)|CK_A;//大小端处理问题
}

/**
 * @brief   发送命令
 * @param   USARTx: where x can be 1, 2, 3, 4, 5, 6, 7 or 8 to select the USART or 
 *          UART peripheral.
 * @param   GPS_Command: GPS命令，参见 GPS command list
 * @param   length: GPS命令的长度，输入形式是sizeof(GPS_Command)
 **/
void GPS_CommandSend(USART_TypeDef* USARTx,uint8_t* GPS_Command,uint8_t length)
{
    //计算弗莱彻校验
    uint16_t checksum;
    checksum=GPS_Checksum(GPS_Command,length);
    //先发送除了校验字节的部分
    for (uint8_t i = 0; i < length; i++)
    {
        USART_SendData(USARTx,GPS_Command[i]);
        while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
    }
    //发送校验
    for (uint8_t i = 0; i < 2; i++)
    {
        USART_SendData(USARTx,((uint8_t*)&checksum)[i]);//留意大小端问题
        while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
    }
}

/**
 * @brief   设置波特率
 * @param   USARTx: where x can be 1, 2, 3, 4, 5, 6, 7 or 8 to select the USART or 
 *          UART peripheral.
 * @param   baud: 波特率
 *           This parameter can be any combination of the following values:
 *             @arg 4800: 
 *             @arg 9600: 
 *             @arg 38400: 
 *             @arg 115200: 
 *             @arg 921600: 
 **/
void GPS_BaudSet(USART_TypeDef* USARTx,uint32_t baud)
{
    //计算填写的值
    for (uint8_t i = 0; i < 4; i++)
    {
        BAUD_SET[6+8+i]=((uint8_t*)&baud)[i];
    }
    
    GPS_CommandSend(USARTx,BAUD_SET,sizeof(BAUD_SET));
}

/**
 * @brief   GPS设置是否成功
 * @return  1是成功，0是失败
 **/
bool GPS_IsCfgSuccess()
{
//UBX-ACK-ACK
//-------------------|SYNC CHAR|-cls|-msg|--LENGTH-|-cls|-msg|CK_A CK_B|
// CFG_SUCCESS[]    ={0xB5,0x62,0x05,0x01,0x02,0x00,0xXX,0xXX}
//UBX-ACK-NAK
//-------------------|SYNC CHAR|-cls|-msg|--LENGTH-|-cls|-msg|CK_A CK_B|
// CFG_FAILUER[]    ={0xB5,0x62,0x05,0x00,0x02,0x00,0xXX,0xXX}
}

/**
 * @brief   GPS解码
 * @param   GPS_DataBuf: 全局变量，解码缓存区的数据，有可能是导航数据或者CFG的设置响应数据
 **/
void GPS_Decode(void)
{
    //先校验
    GPS_Checksum(GPS_DataBuf,GPS_DataBuf[4]+6);

}