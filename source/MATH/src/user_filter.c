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
    if(Filter_CompParamStruct->Ki>0.0f)
    {
        Filter_CompInfoStruct->ErrorInt+=Filter_CompParamStruct->Ki*
                      Filter_CompInfoStruct->Error*(1.0f / sampleFreq);
    }
    else
    {
        Filter_CompInfoStruct->ErrorInt=0;
    }
    Filter_CompInfoStruct->Input+=Filter_CompInfoStruct->ErrorInt+
                                  Filter_CompParamStruct->Kp*
                                  Filter_CompInfoStruct->Error;
    //积分
    Filter_CompInfoStruct->Output=Filter_CompInfoStruct->ErrorInt+Filter_CompInfoStruct->Input*(1.0f / sampleFreq);
}