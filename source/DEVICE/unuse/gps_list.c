// #include "gps.h"

// List_NodeTypeDef* List_HeadPointer=NULL;
// static GPS_StateTypeDef gpsState=GPS_StateFinish;//参考GPS_StateTypeDef
// GPS_DataTypeDef GPS_DataStruct;//GPS获取到的所有数据


// #define BN_880      //北天通讯 GNSS+罗盘模块
// // #define M8N         //pixhawk GPS+罗盘模块
// // #define ATGM336H    //中科微电子 GPS

// //只获取RMC

// #define Km_per_h2m_per_h    (1000.f/3600.f)
// #define m_per_h2Km_per_h    (3600.f/1000.f)

// /**
//  * @brief   GPS初始化，走USART4
//  **/
// void GPS_Init(void)
// {
// #if defined(BN_880)||defined(M8N)//ublox芯片

// /* BN_880 || M8N */
// #elif defined(ATGM336H)//中科微电子芯片

// /* ATGM336H */
// #endif

//     // // //初始化中断线0，用于接受完GPS数据序列之后进行解码
//     // EXTI_InitTypeDef EXTI_InitStruct;
//     // EXTI_InitStruct.EXTI_Line=EXTI_Line0;
//     // EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
//     // // EXTI_InitStruct.EXTI_Trigger=//软件中断，不需要外部输入线
//     // EXTI_InitStruct.EXTI_LineCmd=ENABLE;
//     // EXTI_Init(&EXTI_InitStruct);

//     // // //初始化中断线0的NVIC
//     // NVIC_InitTypeDef NVIC_InitStruct;
//     // NVIC_InitStruct.NVIC_IRQChannel=EXTI0_IRQn;
//     // NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=3;
//     // NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;
//     // NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
//     // NVIC_Init(&NVIC_InitStruct);

//     UART4_Init();
// }

// /**
//  * @brief   GPS解码，将数据存储到对应结构体
//  **/
// void GPS_Decode(void)
// {
//     //和校验检测
//     if(GPS_CheckSum())
//     {
//         //创建临时指针指向链表头节点
//         List_NodeTypeDef* tmpPointer=List_HeadPointer;
//         //判断卫星种类 忽略 过两个有效节点
//         tmpPointer=tmpPointer->List_NextNode->List_NextNode->List_NextNode;
//         //判断内容种类
//         uint32_t tmpVal=0;
//         while(tmpPointer->List_NodeData!=',')
//         {
//             tmpVal=tmpVal<<8;
//             tmpVal|=tmpPointer->List_NodeData;
//             tmpPointer=tmpPointer->List_NextNode;
//         }
//         //跳出循环的时候指向','
//         //根据内容种类和卫星种类将数据分配到不同结构体中，判断逗号作为分隔符
//         switch (tmpVal)
//         {
//         case GPS_ContZDA:
//             // GPS_DecodeZDA(tmpPointer);
//             RGBLED_StateSet(RGBLED_White,RGBLED_1sMode);
//             break;
//         case GPS_ContGGA:
//             // GPS_DecodeGGA(tmpPointer);
//             RGBLED_StateSet(RGBLED_Yellow,RGBLED_1sMode);
//             break;
//         case GPS_ContGLL:
//             RGBLED_StateSet(RGBLED_Purple,RGBLED_1sMode);
//             break;
//         case GPS_ContVTG:
//             RGBLED_StateSet(RGBLED_Red,RGBLED_1sMode);
//             break;
//         case GPS_ContGSA:
//             RGBLED_StateSet(RGBLED_Cyan,RGBLED_1sMode);
//             break;
//         case GPS_ContGSV:
//             RGBLED_StateSet(RGBLED_Green,RGBLED_1sMode);
//             break;
//         case GPS_ContRMC:
//             GPS_DecodeRMC(tmpPointer);
//             RGBLED_StateSet(RGBLED_Blue,RGBLED_1sMode);
//             break;
//         case GPS_ContANT:
//             RGBLED_StateSet(RGBLED_White,RGBLED_1sMode);
//             break;
//         default:
//             RGBLED_StateSet(RGBLED_Black,RGBLED_1sMode);
//             break;
//         }
//         //串口接收到的内容应该动态销毁
//         //解码完成
//         List_DeleteList(&List_HeadPointer);
//         gpsState=GPS_StateFinish;
//     }
//     else
//     {
//         //和校验出错，销毁链表，GPS重新接收数据
//         List_DeleteList(&List_HeadPointer);
//         gpsState=GPS_StateFinish;
//     }
// }

// /**
//  * @brief   信号和校验，采用异或计算，计算范围为$~*之间
//  * @param   str: 字符串指针，相对地址，同时也作为返回值
//  * @return  true表示和校验成功，false表示和校验失败
//  **/
// bool GPS_CheckSum(void)
// {
//     //临时链表指针
//     List_NodeTypeDef* tmp;
//     tmp=List_HeadPointer->List_NextNode;
//     //异或校验
//     uint8_t sum1=0;//计算异或和校验
//     uint8_t sum2;//获取GPS发来的和校验值
//     while(tmp->List_NodeData!='*')
//     {
//         sum1^=tmp->List_NodeData;
//         tmp=tmp->List_NextNode;
//     }
//     //*号后一位
//     tmp=tmp->List_NextNode;
//     sum2=GPS_ASCII2Hex(tmp->List_NodeData)<<4;
//     tmp=tmp->List_NextNode;
//     sum2+=GPS_ASCII2Hex(tmp->List_NodeData);
//     if(sum1==sum2)
//         return true;
//     else
//         return false;
// }

// /**
//  * @brief   解码ZDA序列，主要是UTC时间和日期
//  **/
// void GPS_DecodeZDA(List_NodeTypeDef* NodePointer)
// {
//     // NodePointer->List_NextNode;//跳过','
//     // while(NodePointer->List_NodeData!=',')
//     // {
        
//     // }
// }

// /**
//  * @brief   解码RMC序列，日期，时间，经纬度，对地速度，方位角，磁偏角
//  **/
// void GPS_DecodeRMC(List_NodeTypeDef* NodePointer)
// {
//     NodePointer=NodePointer->List_NextNode;
//     if(NodePointer->List_NodeData!=',')//UTC时间
//     {
//         //解码
//         GPS_ASCII2Time(NodePointer,&GPS_DataStruct.GPS_TimeStruct);
//         //指针走
//         while(NodePointer->List_NodeData!=',')
//         {
//             NodePointer=NodePointer->List_NextNode;
//         }
//     }
//     NodePointer=NodePointer->List_NextNode;
//     if(NodePointer->List_NodeData!=',')//数据是否有效
//     {
//         if(NodePointer->List_NodeData=='V')
//             return;
//         while(NodePointer->List_NodeData!=',')
//         {
//             NodePointer=NodePointer->List_NextNode;
//         }
//     }
//     NodePointer=NodePointer->List_NextNode;
//     if(NodePointer->List_NodeData!=',')//纬度
//     {
//         //解码
//         float_t latitude;
//         latitude=GPS_ASCII2Angel(NodePointer);
//         while(NodePointer->List_NodeData!=',')
//         {
//             NodePointer=NodePointer->List_NextNode;
//         }
//         NodePointer=NodePointer->List_NextNode;
//         if(NodePointer->List_NodeData!=',')//南北纬
//         {
//             if(NodePointer->List_NodeData=='S')
//                 GPS_DataStruct.GPS_LocationStruct.GPS_Latitude=-latitude;
//             else if(NodePointer->List_NodeData=='N')
//                 GPS_DataStruct.GPS_LocationStruct.GPS_Latitude=latitude;
//         }
//         while(NodePointer->List_NodeData!=',')
//         {
//             NodePointer=NodePointer->List_NextNode;
//         }
//     }
//     NodePointer=NodePointer->List_NextNode;
//     if(NodePointer->List_NodeData!=',')//经度
//     {
//         //解码
//         float_t longitude;
//         longitude=GPS_ASCII2Angel(NodePointer);
//         while(NodePointer->List_NodeData!=',')
//         {
//             NodePointer=NodePointer->List_NextNode;
//         }
//         NodePointer=NodePointer->List_NextNode;
//         if(NodePointer->List_NodeData!=',')//东西经
//         {
//             if(NodePointer->List_NodeData=='W')
//                 GPS_DataStruct.GPS_LocationStruct.GPS_Longitude=-longitude;
//             else if(NodePointer->List_NodeData=='E')
//                 GPS_DataStruct.GPS_LocationStruct.GPS_Longitude=longitude;
//         }
//         while(NodePointer->List_NodeData!=',')
//         {
//             NodePointer=NodePointer->List_NextNode;
//         }
//     }
//     NodePointer=NodePointer->List_NextNode;
//     if(NodePointer->List_NodeData!=',')//对地速度
//     {
//         float_t speed;
//         //解码
//         speed=GPS_ASCII2Float(NodePointer);
//         speed*=1852.f/3600.f;
//         GPS_DataStruct.GPS_LocationStruct.GPS_GroundSpeed=speed;
//     }
//     while(NodePointer->List_NodeData!=',')
//     {
//         NodePointer=NodePointer->List_NextNode;
//     }
//     NodePointer=NodePointer->List_NextNode;
//     if(NodePointer->List_NodeData!=',')//方位角
//     {
//         //解码
//         GPS_DataStruct.GPS_LocationStruct.GPS_Azimuth=GPS_ASCII2Float(NodePointer);
//     }
//     while(NodePointer->List_NodeData!=',')
//     {
//         NodePointer=NodePointer->List_NextNode;
//     }
//     NodePointer=NodePointer->List_NextNode;
//     if(NodePointer->List_NodeData!=',')//日期
//     {
//         //解码
//         GPS_ASCII2Date(NodePointer,&GPS_DataStruct.GPS_TimeStruct);
//     }
//     while(NodePointer->List_NodeData!=',')
//     {
//         NodePointer=NodePointer->List_NextNode;
//     }
// }

// /**
//  * @brief   解码GGA序列，时间，位置，定位数据
//  **/
// void GPS_DecodeGGA(List_NodeTypeDef* NodePointer)
// {
//     NodePointer=NodePointer->List_NextNode;
//     if(NodePointer->List_NodeData!=',')//UTC时间
//     {
//         //解码
//         GPS_ASCII2Time(NodePointer,&GPS_DataStruct.GPS_TimeStruct);
//         //指针走
//         while(NodePointer->List_NodeData!=',')
//         {
//             NodePointer=NodePointer->List_NextNode;
//         }
//     }
//     NodePointer=NodePointer->List_NextNode;
//     if(NodePointer->List_NodeData!=',')//纬度
//     {
//         //解码
//         float_t latitude;
//         latitude=GPS_ASCII2Angel(NodePointer);
//         while(NodePointer->List_NodeData!=',')
//         {
//             NodePointer=NodePointer->List_NextNode;
//         }
//         NodePointer=NodePointer->List_NextNode;
//         if(NodePointer->List_NodeData!=',')//南北纬
//         {
//             if(NodePointer->List_NodeData=='S')
//                 GPS_DataStruct.GPS_LocationStruct.GPS_Latitude=-latitude;
//             else if(NodePointer->List_NodeData=='N')
//                 GPS_DataStruct.GPS_LocationStruct.GPS_Latitude=latitude;
//         }
//         while(NodePointer->List_NodeData!=',')
//         {
//             NodePointer=NodePointer->List_NextNode;
//         }
//     }
//     NodePointer=NodePointer->List_NextNode;
//     if(NodePointer->List_NodeData!=',')//经度
//     {
//         //解码
//         float_t longitude;
//         longitude=GPS_ASCII2Angel(NodePointer);
//         while(NodePointer->List_NodeData!=',')
//         {
//             NodePointer=NodePointer->List_NextNode;
//         }
//         NodePointer=NodePointer->List_NextNode;
//         if(NodePointer->List_NodeData!=',')//东西经
//         {
//             if(NodePointer->List_NodeData=='W')
//                 GPS_DataStruct.GPS_LocationStruct.GPS_Longitude=-longitude;
//             else if(NodePointer->List_NodeData=='E')
//                 GPS_DataStruct.GPS_LocationStruct.GPS_Longitude=longitude;
//         }
//         while(NodePointer->List_NodeData!=',')
//         {
//             NodePointer=NodePointer->List_NextNode;
//         }
//     }
//     NodePointer=NodePointer->List_NextNode;
//     if(NodePointer->List_NodeData!=',')//定位模式
//     {
//         if(NodePointer->List_NodeData=='0')
//         {
//             GPS_DataStruct.GPS_AccuracyStruct.GPS_QltyFactor=GPS_QltyFactorUnlocated;
//             return;
//         }
//         else if(NodePointer->List_NodeData=='1')
//             GPS_DataStruct.GPS_AccuracyStruct.GPS_QltyFactor=GPS_QltyFactorRGPS;
//         else if(NodePointer->List_NodeData=='2')
//             GPS_DataStruct.GPS_AccuracyStruct.GPS_QltyFactor=GPS_QltyFactorDGPS;
//         else if(NodePointer->List_NodeData=='3')
//             GPS_DataStruct.GPS_AccuracyStruct.GPS_QltyFactor=GPS_QltyFactorPPS;
//         while(NodePointer->List_NodeData!=',')
//         {
//             NodePointer=NodePointer->List_NextNode;
//         }
//     }
//     NodePointer=NodePointer->List_NextNode;
//     if(NodePointer->List_NodeData!=',')//卫星数量
//     {
//         uint8_t tmp;
//         tmp=GPS_ASCII2Hex(NodePointer->List_NodeData)*10;
//         NodePointer=NodePointer->List_NextNode;
//         tmp+=GPS_ASCII2Hex(NodePointer->List_NodeData);
//         GPS_DataStruct.GPS_AccuracyStruct.GPS_SatNum=tmp;
//         while(NodePointer->List_NodeData!=',')
//         {
//             NodePointer=NodePointer->List_NextNode;
//         }
//     }
//     NodePointer=NodePointer->List_NextNode;
//     if(NodePointer->List_NodeData!=',')//HDOP水平精度因子
//     {
//         GPS_DataStruct.GPS_AccuracyStruct.GPS_HDOP=GPS_ASCII2Float(NodePointer);
//         while(NodePointer->List_NodeData!=',')
//         {
//             NodePointer=NodePointer->List_NextNode;
//         }
//     }
//     NodePointer=NodePointer->List_NextNode;
//     if(NodePointer->List_NodeData!=',')//海拔高度
//     {
//         GPS_DataStruct.GPS_LocationStruct.GPS_Altitude=GPS_ASCII2Float(NodePointer);
//         while(NodePointer->List_NodeData!=',')
//         {
//             NodePointer=NodePointer->List_NextNode;
//         }
//         NodePointer=NodePointer->List_NextNode;//跳过','
//         NodePointer=NodePointer->List_NextNode;//跳过'M'
//     }
//     NodePointer=NodePointer->List_NextNode;
//     if(NodePointer->List_NodeData!=',')//高程
//     {
//         GPS_DataStruct.GPS_LocationStruct.GPS_Elevation=GPS_ASCII2Float(NodePointer);
//         while(NodePointer->List_NodeData!=',')
//         {
//             NodePointer=NodePointer->List_NextNode;
//         }
//         NodePointer=NodePointer->List_NextNode;//跳过','
//         NodePointer=NodePointer->List_NextNode;//跳过'M'
//     }
// }

// /**
//  * @brief   字符串转十六进制
//  **/
// uint8_t GPS_ASCII2Hex(uint8_t input)
// {
//     if((input>='0')&&(input<='9'))//数字字符
//         input-=0x30;
//     else if((input>='A')&&(input<='Z'))//大写字母字符
//         input-=0x37;
//     else if((input>='a')&&(input<='z'))//小写字母字符
//         input-=0x57;
//     return input;
// }

// /**
//  * @brief   ASCII转度，用于经纬度转换
//  **/
// float_t GPS_ASCII2Angel(List_NodeTypeDef* NodePointer)
// {
//     uint16_t tmp1=0;
//     float_t tmp2=0;
//     uint8_t cnt=0;
//     while(NodePointer->List_NodeData!='.')
//     {
//         tmp1*=10;
//         tmp1+=GPS_ASCII2Hex(NodePointer->List_NodeData);
//         NodePointer=NodePointer->List_NextNode;
//     }
//     tmp2=tmp1%100;
//     tmp1/=100;
//     NodePointer=NodePointer->List_NextNode;//跳过'.'
//     while(NodePointer->List_NodeData!=',')
//     {
//         tmp2*=10;
//         tmp2+=GPS_ASCII2Hex(NodePointer->List_NodeData);
//         NodePointer=NodePointer->List_NextNode;
//         cnt++;
//     }
//     while(cnt!=0)
//     {
//         tmp2/=10;
//         cnt--;
//     }
//     tmp2/=60;
//     tmp2+=tmp1;
//     return tmp2;
// }

// /**
//  * @brief   ASCII码转浮点数
//  * @param   str: 字符串的指针
//  * @return  float: 浮点数
//  **/
// float_t GPS_ASCII2Float(List_NodeTypeDef* NodePointer)
// {
//     float_t tmp1=0;
//     float_t tmp2=0;
//     uint8_t flag=0;
//     if(NodePointer->List_NodeData=='-')
//     {
//         NodePointer=NodePointer->List_NextNode;
//         flag=1;
//     }
//     while(NodePointer->List_NodeData!='.')
//     {
//         tmp1*=10;
//         tmp1+=GPS_ASCII2Hex(NodePointer->List_NodeData);
//         NodePointer=NodePointer->List_NextNode;
//     }
//     NodePointer=NodePointer->List_NextNode;//跳过'.'
//     while(NodePointer->List_NodeData!=',')
//     {
//         tmp2*=10;
//         tmp2+=GPS_ASCII2Hex(NodePointer->List_NodeData);
//         NodePointer=NodePointer->List_NextNode;
//     }
//     while(tmp2>=1)
//     {
//         tmp2/=10;
//     }
//     tmp1+=tmp2;
//     if(flag)
//         tmp1*=-1;
//     return tmp1;
// }

// /**
//  * @brief   ASCII码转UTC时间
//  * @param   NodePointer: 链表指针的指针，因为要修改指针的值
//  * @param   GPS_TimeStruct: GPS获取时间结构体
//  * @return  位移后的指针地址
//  **/
// void GPS_ASCII2Time(List_NodeTypeDef* NodePointer,GPS_TimeTypeDef* GPS_TimeStruct)
// {
//     //hh
//     GPS_TimeStruct->GPS_Hour=GPS_ASCII2Hex(NodePointer->List_NodeData)*10+
//                              GPS_ASCII2Hex(NodePointer->List_NextNode->List_NodeData);
//     NodePointer=NodePointer->List_NextNode->List_NextNode;
//     //mm
//     GPS_TimeStruct->GPS_Minute=GPS_ASCII2Hex(NodePointer->List_NodeData)*10+
//                                GPS_ASCII2Hex(NodePointer->List_NextNode->List_NodeData);
//     NodePointer=NodePointer->List_NextNode->List_NextNode;
//     //ss
//     GPS_TimeStruct->GPS_Second=GPS_ASCII2Hex(NodePointer->List_NodeData)*10+
//                                GPS_ASCII2Hex(NodePointer->List_NextNode->List_NodeData);
//     NodePointer=NodePointer->List_NextNode->List_NextNode;
//     //跳过'.'
//     NodePointer=NodePointer->List_NextNode;
//     //msmsms
//     GPS_TimeStruct->GPS_Millisecond=(uint16_t)
//                                     GPS_ASCII2Hex(NodePointer->List_NodeData)*10+
//                                     GPS_ASCII2Hex(NodePointer->List_NextNode->List_NodeData);
// }

// /**
//  * @brief   ASCII码转日期
//  * @param   str: 字符串的指针，相对地址
//  * @param   GPS_TimeStruct: GPS获取时间结构体
//  * @return  位移后的指针地址
//  **/
// void GPS_ASCII2Date(List_NodeTypeDef* NodePointer,GPS_TimeTypeDef* GPS_TimeStruct)
// {
//     //dd
//     GPS_TimeStruct->GPS_Day=GPS_ASCII2Hex(NodePointer->List_NodeData)*10+
//                             GPS_ASCII2Hex(NodePointer->List_NextNode->List_NodeData);
//     NodePointer=NodePointer->List_NextNode->List_NextNode;
//     //mm
//     GPS_TimeStruct->GPS_Minute=GPS_ASCII2Hex(NodePointer->List_NodeData)*10+
//                                GPS_ASCII2Hex(NodePointer->List_NextNode->List_NodeData);
//     NodePointer=NodePointer->List_NextNode->List_NextNode;
//     //yy
//     GPS_TimeStruct->GPS_Year=GPS_ASCII2Hex(NodePointer->List_NodeData)*10+
//                              GPS_ASCII2Hex(NodePointer->List_NextNode->List_NodeData);
//     NodePointer=NodePointer->List_NextNode->List_NextNode;
// }

// /**
//  * @brief   检测是否为有效参数
//  * @param   NodePointer: 链表指针
//  * @return  长度，0长度代表无效
//  **/
// uint8_t GPS_IsValidData(List_NodeTypeDef* NodePointer)
// {
//     uint8_t length=0;
//     NodePointer=NodePointer->List_NextNode;//跳过上一个检测的','
//     while (NodePointer->List_NodeData!=',')
//     {
//         NodePointer=NodePointer->List_NextNode;
//         length++;
//     }
//     return length;
// }

// /**
//  * @brief   UART4中断处理函数，用于GPS数据接收
//  **/
// void UART4_IRQHandler(void)
// {
//     if(USART_GetFlagStatus(UART4,USART_FLAG_RXNE)!=RESET)//接收到数据，进入中断
//     {
//         uint8_t tmp;
//         tmp=USART_ReceiveData(UART4);
//         switch (tmp)
//         {
//         case '$'://起始序列符号
//             if(gpsState==GPS_StateFinish)//前一次的数据流解码已经完成
//             {
//                 gpsState=GPS_StateStart;
//                 //创建链表
//                 List_CreatHead(&List_HeadPointer);
//             }
//             break;
//         case 0x0D://可能是回车或者校验位
//             if(gpsState==GPS_StateStart)
//                 gpsState=GPS_StatePrepEnd;
//             break;
//         case 0x0A://可能是换行或者校验位
//             switch (gpsState)
//             {
//             case GPS_StateStart:
//                 //写入链表
//                 List_AddNode(List_HeadPointer,tmp);
//                 break;
//             case GPS_StatePrepEnd://前一个值是0x0D
//                 gpsState=GPS_StateEnd;
//                 //触发中断
//                 //这两行是测试用的
//                 USART_Cmd(UART4,DISABLE);//解码前关掉串口
//                 GPS_Decode();
//                 USART_Cmd(UART4,ENABLE);//开串口
//                 // EXTI_GenerateSWInterrupt(EXTI_Line0);
//                 //关闭串口
//                 // USART_Cmd(UART4,DISABLE);
//                 // List_DeleteList(&List_HeadPointer);
//                 // RGBLED_StateSet(RGBLED_Cyan,RGBLED_1sMode);
//                 gpsState=GPS_StateFinish;
//                 //开串口
                
//                 break;
//             default:
//                 break;
//             }
//             break;
//         default:
//             switch (gpsState)
//             {
//             case GPS_StateStart:
//                 //写入链表
//                 List_AddNode(List_HeadPointer,tmp);
//                 break;
//             case GPS_StatePrepEnd://前一个值是0x0D
//                 //0x0D写入链表
//                 List_AddNode(List_HeadPointer,0x0D);
//                 //这次获取的值也写入链表
//                 List_AddNode(List_HeadPointer,tmp);
//                 gpsState=GPS_StateStart;
//                 break;
//             default:
//                 break;
//             }
//             break;
//         }
//         //清除标志位
//         USART_ClearFlag(UART4,USART_FLAG_RXNE);
//     }
//     if(USART_GetFlagStatus(UART4,USART_FLAG_ORE)!=RESET)
//     {
//         USART_ClearFlag(UART4,USART_FLAG_ORE);
//     }
// }

// // /**
// //  * @brief   外部中断0进行GPS链表解码
// //  **/
// // void EXTI0_IRQHandler(void)
// // {
// //     if(EXTI_GetFlagStatus(EXTI_Line0)!=RESET)//PWM输入发生触发
// //     {
// //         // gpsState=GPS_StateDecode;
// //         // GPS_Decode();
// //         EXTI_ClearFlag(EXTI_Line0);
// //     }
// // }