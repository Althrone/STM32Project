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
    Filter_CompInfoStruct->Output+=(Filter_CompInfoStruct->Input+
                                    Filter_CompInfoStruct->ErrorInt+
                                    Filter_CompParamStruct->Kp*
                                    Filter_CompInfoStruct->Error)*
                                   (1.0f / sampleFreq);
}

/**
 * @brief   滑动窗口滤波，使用的时候请在此函数前申请一个数组空间
 * @param   bufAddr: 缓存空间首地址
 * @param   length: 缓存空间长度
 * @param   val: 最新的输入值
 * @return  返回滤波后的值
 **/
float_t Filter_Avg(float_t* bufAddr,uint16_t length,float_t val)
{
    //数组位移
    float_t tmp=0;
    for(uint16_t i=length;i>1;i--)
    {
        *(bufAddr+i-1)=*(bufAddr+i-2);
        tmp+=*(bufAddr+i-1);
    }
    *bufAddr=val;
    tmp+=val;
    return tmp/length;
}