#include "car.h"

void Car_DroneCtrl(ANO_DT_SendRCDataTypeDef* ANO_DT_SendRCDataStruct)
{
    //油门控制舵机
    Steer_Test(ANO_DT_SendRCDataStruct);
    //俯仰控制前进后退，横滚控制左右
}