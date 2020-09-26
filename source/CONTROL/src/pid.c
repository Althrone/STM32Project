#include "pid.h"

/**
 * @brief   增量式PID控制器
 * @param   PID_InfoStruct: 控制器输入输出信号
 * @param   PID_ParamStruct: PID参数
 * @param   target: 目标值
 * @param   measure: 测量值
 **/
void PID_PostionCON(PID_InfoTypeDef* PID_InfoStruct,
                       PID_ParamTypeDef* PID_ParamStruct,
                       float_t target,float_t measure)
{
    //上一次输出继承到这次
    PID_InfoStruct->PrevOutput=PID_InfoStruct->Output;
    //上一次以及上上次输入误差
    PID_InfoStruct->PrevPrevError=PID_InfoStruct->PrevError;
    PID_InfoStruct->PrevError=PID_InfoStruct->Error;
    PID_InfoStruct->Error=target-measure;
    PID_InfoStruct->Output=PID_InfoStruct->PrevOutput+
                           PID_ParamStruct->P*(PID_InfoStruct->Error-PID_InfoStruct->PrevError)+
                           PID_ParamStruct->I*PID_InfoStruct->Error+
                           PID_ParamStruct->D*(PID_InfoStruct->Error-2*PID_InfoStruct->PrevError+PID_InfoStruct->PrevPrevError);
}

/**
 * @brief   将角速度环，角度环PID参数写入AT24C02
 **/
void PID_ParamSet()
{
    
}

/**
 * @brief   增量式PID控制器
 * @param   PID_InfoStruct: 控制器输入输出信号
 * @param   PID_ParamStruct: PID参数
 * @param   target: 目标值
 * @param   measure: 测量值
 **/
// void PID_IncrementCON(PID_InfoTypeDef* PID_InfoStruct,
//                       PID_ParamTypeDef* PID_ParamStruct,
//                       float_t target,float_t measure)