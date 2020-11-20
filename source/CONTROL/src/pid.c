#include "pid.h"

//对应的pid参数
PID_ParamTypeDef PID_RollAngleParam;
PID_ParamTypeDef PID_PitchAngleParam;

PID_ParamTypeDef PID_RollRateParam;
PID_ParamTypeDef PID_PitchRateParam;
PID_ParamTypeDef PID_YawRateParam;

/**
 * @brief   增量式PID控制器
 * @param   PID_InfoStruct: 控制器输入输出信号
 * @param   PID_ParamStruct: PID参数
 * @param   target: 目标值
 * @param   measure: 测量值
 * @param   ANO_DT_SendRCDataStruct: 遥控器的值，用于积分分离
 **/
void PID_IncCtrl(PID_InfoTypeDef* PID_InfoStruct,
                 PID_ParamTypeDef* PID_ParamStruct,
                 ANO_DT_SendRCDataTypeDef* ANO_DT_SendRCDataStruct,
                 float_t target,float_t measure)
{
    //上一次输出继承到这次
    PID_InfoStruct->PrevOutput=PID_InfoStruct->Output;

    //上一次以及上上次输入误差
    PID_InfoStruct->PrevPrevError=PID_InfoStruct->PrevError;
    PID_InfoStruct->PrevError=PID_InfoStruct->Error;
    PID_InfoStruct->Error=target-measure;

    //积分分离
    if(ANO_DT_SendRCDataStruct->ANO_DT_RCThrottle<=1200)
    {
        PID_InfoStruct->Error=0;
    }

    float_t I;
    I=PID_ParamStruct->I*PID_InfoStruct->Error;
    //积分限幅
    if(I>150)
        I=150;
    else if (I<-150)
    {
        I=-150;
    }

    PID_InfoStruct->Output=PID_InfoStruct->PrevOutput+
                          PID_ParamStruct->P*(PID_InfoStruct->Error-PID_InfoStruct->PrevError)+
                          I+
                          PID_ParamStruct->D*(PID_InfoStruct->Error-2*PID_InfoStruct->PrevError+PID_InfoStruct->PrevPrevError);
}

/**
 * @brief   将角速度环，角度环PID参数写入AT24C02
 **/
void PID_ParamSet(void)
{
    
}

/**
 * @brief   手动初始化PID参数
 **/
void PID_ParamInit(void)
{
    //横滚角度环
    // PID_RollAngleParam.P=1.3;
    // PID_RollAngleParam.I=0;
    // PID_RollAngleParam.D=0.1;

    PID_RollAngleParam.P=0;
    PID_RollAngleParam.I=0;
    PID_RollAngleParam.D=0;

    //俯仰角度环
    // PID_PitchAngleParam.P=1.3;
    // PID_PitchAngleParam.I=0;
    // PID_PitchAngleParam.D=0.1;

    PID_PitchAngleParam.P=0;
    PID_PitchAngleParam.I=0;
    PID_PitchAngleParam.D=0;

    /***************************************/

    //横滚角速度环
    // PID_RollRateParam.P=1.2;//2~2.5等幅振荡，加I之后乘0.6//1.2
    // PID_RollRateParam.I=0.1;//0.1
    // PID_RollRateParam.D=0.5;//0.5
    
    PID_RollRateParam.P=2.1;
    PID_RollRateParam.I=0.05;
    PID_RollRateParam.D=0;

    //俯仰角速度环
    // PID_PitchRateParam.P=1.2;
    // PID_PitchRateParam.I=0.1;
    // PID_PitchRateParam.D=0.5;

    PID_RollRateParam.P=2.1;
    PID_RollRateParam.I=0.05;
    PID_RollRateParam.D=0;

    //偏航角速度环
    PID_YawRateParam.P=0;
    PID_YawRateParam.I=0;
    PID_YawRateParam.D=0;
}

/**
 * @brief   位置式PID控制器
 * @param   PID_InfoStruct: 控制器输入输出信号
 * @param   PID_ParamStruct: PID参数
 * @param   target: 目标值
 * @param   measure: 测量值
 **/
// void PID_PostionCtrl(PID_InfoTypeDef* PID_InfoStruct,
//                      PID_ParamTypeDef* PID_ParamStruct,
//                      float_t target,float_t measure)