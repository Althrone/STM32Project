#include "base.h"

//基础快速算法库

/**
 * @brief  平方根倒数速算法，计算1/x^(1/2)
 * @param  input: 
 **/
float_t Fast_InvSqrt(float_t x)
{
    float_t y=x;
    float_t halfx=x*0.5f;
    int32_t i=*(int32_t*)&y;
    // i=0x5f3759df-(i>>1);
    i=0x5f375a86-(i>>1);//1－2次牛顿迭代后能得到最大精度的R值
    y=*(float*)&i;
    y=y*(1.5f-(halfx*y*y));//牛顿法迭代
    // y=y*(1.5f-(halfx*y*y));//二次迭代
    return y;
}

/**
 * @brief   递推法计算均值
 * @param   mean: 目前的平均值
 * @param   x: 需要累加的数据
 * @param   n: 第几个累加数据，从1开始
 **/
float_t Recursion_Mean(float_t mean,float_t x,uint16_t n)
{
    mean=mean+(x-mean)/n;
    return mean;
}