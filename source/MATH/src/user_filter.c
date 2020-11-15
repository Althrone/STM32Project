#include "user_filter.h"

// static float_t integral;
#define sampleFreq	100.0f			// sample frequency in Hz

/**
 * @brief   互补滤波
 * @param   Filter_CompParamTypeDef: C(s)参数，Kp，Ki
 * @param   Filter_CompInfoTypeDef: 输入输出参数
 **/
void Filter_Comp(Filter_CompParamTypeDef* Filter_CompParamStruct,
                 Filter_CompInfoTypeDef* Filter_CompInfoStruct)
{
    float_t tmp_integral;//临时积分项，通过传入参数参与积分
    if(Filter_CompParamStruct->Ki>0.0f)
    {
        tmp_integral+=Filter_CompParamStruct->Ki*
                      Filter_CompInfoStruct->Error*(1.0f / sampleFreq);
    }
    else
    {
        tmp_integral=0;
    }
    Filter_CompInfoStruct->Input+=tmp_integral+
                                  Filter_CompParamStruct->Kp*
                                  Filter_CompInfoStruct->Error;
    //积分
    Filter_CompInfoStruct->Output=tmp_integral+Filter_CompInfoStruct->Input;
}

// /**
//  * @brief   三阶互补滤波
//  **/
// void Filter_3OrderComp()