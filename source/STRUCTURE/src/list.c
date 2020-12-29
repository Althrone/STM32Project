#include "list.h"

//链表测试：通过
// List_NodeTypeDef* List_HeadPointer=NULL;
// if(List_CreatHead(&List_HeadPointer))
// {
//     if(List_AddNode(List_HeadPointer,11))
//     {
//         List_AddNode(List_HeadPointer,33);
//         uint8_t tmp;
//         tmp=List_Length(List_HeadPointer);
//         tmp=List_DeleteList(&List_HeadPointer);
//     }
// }

//仅讨论非循环单链表

/**
 * @brief   创建链表表头（头结点）
 * @param   List_HeadStruct: 指向头结点结构体的指针
 * @return  true表示创建表头成功，false表示创建表头失败
 **/
bool List_CreatHead(List_NodeTypeDef** List_HeadStruct)
{
    *List_HeadStruct=(List_NodeTypeDef*)malloc(sizeof(List_NodeTypeDef));
    if(*List_HeadStruct==NULL)
        return false;

    (*List_HeadStruct)->List_NextNode=NULL;
    return true;
}

/**
 * @brief   删除整个链表，包括头结点
 * @param   List_HeadStruct: 指向头结点结构体的指针
 * @return  true表示删除成功，false表示删除失败
 **/
bool List_DeleteList(List_NodeTypeDef** List_HeadStruct)
{
    //创建一个临时链表节点结构体指针
    List_NodeTypeDef* tmp1;
    List_NodeTypeDef* tmp2;
    tmp1=*List_HeadStruct;//临时指针指向链表头结点
    while (tmp1!=NULL)
    {
        tmp2=tmp1->List_NextNode;
        free(tmp1);
        tmp1=tmp2;
    }
    *List_HeadStruct=NULL;
    return true;
}

/**
 * @brief   追加节点，在最后一个节点的后面增加一个节点
 * @param   List_HeadStruct: 指向头结点结构体的指针
 * @param   data： 追加的数据
 * @return  true表示追加成功，false表示追加失败，也许内存满了
 **/
bool List_AddNode(List_NodeTypeDef* List_HeadStruct,uint8_t data)
{
    //创建一个临时链表节点结构体指针
    List_NodeTypeDef* tmp;
    tmp=List_HeadStruct;//临时指针指向链表头结点
    //判断tmp目前指向的节点的下一节点指针成员是否为空
    while (tmp->List_NextNode!=NULL)
    {
        tmp=tmp->List_NextNode;//不为空，tmp指向下一个节点
    }
    //指针为空，开辟内存空间
    List_NodeTypeDef* List_NewStruct;//新节点
    List_NewStruct=(List_NodeTypeDef*)malloc(sizeof(List_NodeTypeDef));
    if(List_NewStruct==NULL)//内存分配失败
        return false;
    //对新节点结构体成员赋值
    List_NewStruct->List_NodeData=data;
    List_NewStruct->List_NextNode=NULL;//因为追加肯定是最后一个，所以要防止野指针
    //连接上新节点
    tmp->List_NextNode=List_NewStruct;
    return true;
}

/**
 * @brief   插入节点
 * @param   List_HeadStruct: 指向头结点结构体的指针
 * @param   pos: 要插入的节点的位置，0表示头结点之后插入，如果超过链表长度，插入失败
 * @param   data: 插入的数据
 * @return  true表示插入成功，false表示插入失败，也许内存满了
 **/
// bool List_InsertNode(List_NodeTypeDef* List_HeadStruct,uint32_t pos,uint8_t data)
// {

// }

/**
 * @brief   删除任意节点，包括头结点
 * @param   List_HeadStruct: 指向头结点结构体的指针
 * @param   pos: 要删除的第几个节点，0代表头结点，存在有效节点的时候不能删除头结点
 * @return  true表示删除成功，false表示删除失败
 **/
// bool List_DeleteNode(List_NodeTypeDef* List_HeadStruct,uint32_t pos)
// {
    
// }

/**
 * @brief   计算链表有效节点长度
 * @param   List_HeadStruct: 指向头结点结构体的指针
 * @return  链表长度
 **/
uint32_t List_Length(List_NodeTypeDef* List_HeadStruct)
{
    //头结点不算有效节点
    //创建一个临时链表节点结构体指针
    List_NodeTypeDef* tmp;
    tmp=List_HeadStruct;//临时指针指向链表头结点
    uint32_t cnt=0;
    while (tmp->List_NextNode!=NULL)
    {
        tmp=tmp->List_NextNode;
        ++cnt;
    }
    return cnt;
}

/**
 * @brief   链表遍历输出
 * @param   List_HeadStruct: 指向头结点结构体的指针
 * @param   data: 对应链表的数据
 **/
void List_Traverse(List_NodeTypeDef* List_HeadStruct,uint8_t data)
{

}