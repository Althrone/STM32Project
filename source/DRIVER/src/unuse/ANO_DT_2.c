// /**
//  * @file    ANO_DT.c
//  * @author  Althrone
//  * @version 2.0
//  * @date    
//  * @brief   使用匿名飞控V6协议，使用链表进行数据发送。
//  **/

// #include "ANO_DT.h"

// //静态全局变量，只允许在本文件内使用
// static List_NodeTypeDef* ANO_DT_List_HeadPointer=NULL;

// #define S_ADDR  ANO_DT_ToKon
// #define D_ADDR  ANO_DT_Computer

// /**
//  * @brief  结构体成员拆解
//  * @param  memberlenth: 成员长度
//  * @param  memberhead: 成员首地址
//  * @param  ANO_DT_List_HeadPointer: 链表头指针
//  **/
// void ANO_DT_SplitMember(uint8_t memberlenth,uint8_t* memberhead,
//                         List_NodeTypeDef* ANO_DT_List_HeadPointer)
// {
//     memberhead+=memberlenth-1;
//     for (uint8_t i = 0; i < memberlenth; i++)
//     {
//         // *(++databuf)=*memberhead;
//         List_AddNode(ANO_DT_List_HeadPointer,*memberhead);
//         memberhead--;
//     }
// }

// /**
//  * @brief  发送姿态等基本信息
//  * @param  USARTx: 串口号
//  * @param  ANO_DT_SendStatusStruct: 姿态等信息的结构体
//  **/
// void ANO_DT_SendStatus(USART_TypeDef* USARTx,ANO_DT_SendStatusTypeDef* ANO_DT_SendStatusStruct)
// {
//     uint8_t sum=0;
//     //计算数据长度
//     uint8_t lenth=sizeof(ANO_DT_SendStatusStruct->ANO_DT_Roll)+
//                   sizeof(ANO_DT_SendStatusStruct->ANO_DT_Pitch)+
//                   sizeof(ANO_DT_SendStatusStruct->ANO_DT_Yaw)+
//                   sizeof(ANO_DT_SendStatusStruct->ANO_DT_Altitude)+
//                   sizeof(ANO_DT_SendStatusStruct->ANO_DT_FlyModel)+
//                   sizeof(ANO_DT_SendStatusStruct->ANO_DT_Armed);
//     List_CreatHead(&ANO_DT_List_HeadPointer);
//     List_AddNode(ANO_DT_List_HeadPointer,0xAA);
//     List_AddNode(ANO_DT_List_HeadPointer,S_ADDR);
//     List_AddNode(ANO_DT_List_HeadPointer,D_ADDR);
//     List_AddNode(ANO_DT_List_HeadPointer,ANO_DT_Status);
//     List_AddNode(ANO_DT_List_HeadPointer,lenth);
//     //数据填充
//     ANO_DT_SplitMember(sizeof(ANO_DT_SendStatusStruct->ANO_DT_Roll),
//                       (uint8_t *)&ANO_DT_SendStatusStruct->ANO_DT_Roll,
//                       ANO_DT_List_HeadPointer);
//     ANO_DT_SplitMember(sizeof(ANO_DT_SendStatusStruct->ANO_DT_Pitch),
//                       (uint8_t *)&ANO_DT_SendStatusStruct->ANO_DT_Pitch,
//                       ANO_DT_List_HeadPointer);
//     ANO_DT_SplitMember(sizeof(ANO_DT_SendStatusStruct->ANO_DT_Yaw),
//                       (uint8_t *)&ANO_DT_SendStatusStruct->ANO_DT_Yaw,
//                       ANO_DT_List_HeadPointer);
//     ANO_DT_SplitMember(sizeof(ANO_DT_SendStatusStruct->ANO_DT_Altitude),
//                       (uint8_t *)&ANO_DT_SendStatusStruct->ANO_DT_Altitude,
//                       ANO_DT_List_HeadPointer);
//     ANO_DT_SplitMember(sizeof(ANO_DT_SendStatusStruct->ANO_DT_FlyModel),
//                       (uint8_t *)&ANO_DT_SendStatusStruct->ANO_DT_FlyModel,
//                       ANO_DT_List_HeadPointer);
//     ANO_DT_SplitMember(sizeof(ANO_DT_SendStatusStruct->ANO_DT_Armed),
//                       (uint8_t *)&ANO_DT_SendStatusStruct->ANO_DT_Armed,
//                       ANO_DT_List_HeadPointer);
//     List_NodeTypeDef* Tmp_List_HeadPointer;
//     Tmp_List_HeadPointer=ANO_DT_List_HeadPointer->List_NextNode;
//     //和校验
//     while(Tmp_List_HeadPointer!=NULL)
//     {
//         sum=sum+Tmp_List_HeadPointer->List_NodeData;
//         Tmp_List_HeadPointer=Tmp_List_HeadPointer->List_NextNode;
//     }
//     List_AddNode(ANO_DT_List_HeadPointer,sum);
//     //发送数据
//     Tmp_List_HeadPointer=ANO_DT_List_HeadPointer->List_NextNode;
//     free(ANO_DT_List_HeadPointer);
//     ANO_DT_List_HeadPointer=Tmp_List_HeadPointer;
//     while(ANO_DT_List_HeadPointer!=NULL)
//     {
//         while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
//         USART_SendData(USARTx,ANO_DT_List_HeadPointer->List_NodeData);
//         Tmp_List_HeadPointer=ANO_DT_List_HeadPointer->List_NextNode;
//         free(ANO_DT_List_HeadPointer);
//         ANO_DT_List_HeadPointer=Tmp_List_HeadPointer;
//     }
//     // List_DeleteList(&ANO_DT_List_HeadPointer);
// }