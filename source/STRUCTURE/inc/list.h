/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LIST_H
#define __LIST_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

#include <stdbool.h>
#include <stdlib.h>

/* Exported types ------------------------------------------------------------*/
typedef struct List_Node
{
    uint8_t List_NodeData;              //一字节数据
    struct List_Node* List_NextNode;    //指向下一节点
}List_NodeTypeDef;


/* Exported functions --------------------------------------------------------*/

bool List_CreatHead(List_NodeTypeDef** List_HeadStruct);
bool List_DeleteList(List_NodeTypeDef** List_HeadStruct);
bool List_AddNode(List_NodeTypeDef* List_HeadStruct,uint8_t data);
// bool List_InsertNode(List_NodeTypeDef* List_HeadStruct,uint32_t pos,uint8_t data);
// bool List_DeleteNode(List_NodeTypeDef* List_HeadStruct,uint32_t pos);
uint32_t List_Length(List_NodeTypeDef* List_HeadStruct);


#ifdef __cplusplus
}
#endif

#endif /*__LIST_H */