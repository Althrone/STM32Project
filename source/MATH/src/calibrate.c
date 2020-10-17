#include "calibrate.h"

/**
 * @brief   传感器校准
 **/
void CAL_Senser(void)
{
    //进来检查SWA的值是否是2000，不是就退出
    ANO_DT_SendRCDataTypeDef ANO_DT_SendRCDataStruct;
    PPM_GetRCData(&ANO_DT_SendRCDataStruct);
    if(ANO_DT_SendRCDataStruct.ANO_DT_RCAUX1<=1800)
        return;
    //接近2000，就进行下面的计算
    while (ANO_DT_SendRCDataStruct.ANO_DT_RCAUX1>1900)//在此之前SWA拨杆↓，
    {
        PPM_GetRCData(&ANO_DT_SendRCDataStruct);
        RGBLED_StateSet(RGBLED_Red,RGBLED_1sMode);
        if(ANO_DT_SendRCDataStruct.ANO_DT_RCAUX1<1100)//SWA拨杆↑
        {
            RGBLED_StateSet(RGBLED_Yellow,RGBLED_1sMode);
            MPU6050_AccelCal();
        }
    }
    while (ANO_DT_SendRCDataStruct.ANO_DT_RCAUX1<1100)//校准加速度计和磁力计
    {
        PPM_GetRCData(&ANO_DT_SendRCDataStruct);
        RGBLED_StateSet(RGBLED_Blue,RGBLED_1sMode);
    }
    while (ANO_DT_SendRCDataStruct.ANO_DT_RCAUX1>1900)//在此之前SWA拨杆↓，准备修正
    {
        PPM_GetRCData(&ANO_DT_SendRCDataStruct);
        RGBLED_StateSet(RGBLED_Red,RGBLED_1sMode);
        if (ANO_DT_SendRCDataStruct.ANO_DT_RCAUX1<1100)//SWA拨杆↑，进行修正
        {
            RGBLED_StateSet(RGBLED_Yellow,RGBLED_1sMode);
            MPU6050_GyroCal();//获取陀螺仪零偏
            AK8975_GetGeomagneticVector();//获取地磁矢量
            RGBLED_StateSet(RGBLED_Green,RGBLED_1sMode);
        }
    }
    while (1)
    {
        // RGBLED_StateSet(RGBLED_Cyan,RGBLED_1sMode);//矫正完成，显示绿色
    }
}

/**
 * @brief   通用型椭球校正
 * @param   x: 机体坐标系下传感器指向机头的分量
 * @param   y: 机体坐标系下传感器指向机身右侧的分量
 * @param   z: 机体坐标系下传感器指向机身下方的分量
 * @param   i: 采样次数
 * @param   CAL_EllipsoidParamStruct: 校正之后的参数
 **/
void CAL_Ellipsoid(float_t x,float_t y,float_t z,uint8_t i,
                   CAL_EllipsoidParamTypeDef* CAL_EllipsoidParamStruct)
{
    //X*GenMat=Y，[X|Y]增广矩阵
    //求GenMat就是Y*X^(-1)
    /*
      ┌     ┐
      │ y^2 │
      │ z^2 │┌                 ┐
    X=│  x  ││ y^2 z^2 x y z 1 │
      │  y  │└                 ┘
      │  z  │
      │  1  │
      └     ┘
          ┌     ┐
          │ y^2 │
          │ z^2 │
    Y=-x^2│  x  │
          │  y  │
          │  z  │
          │  1  │
          └     ┘
           ┌             ┐T
    GenMat=│ a b c d e f │
           └             ┘
    */
    ANO_DT_SendRCDataTypeDef ANO_DT_SendRCDataStruct;
    for (uint8_t j = 0; j < 10; j++)
    {
        SysTick_DelayMs(1);
        PPM_GetRCData(&ANO_DT_SendRCDataStruct);
    }
    while (ANO_DT_SendRCDataStruct.ANO_DT_RCAUX1>1900)
    {
        PPM_GetRCData(&ANO_DT_SendRCDataStruct);
        RGBLED_StateSet(RGBLED_Red,RGBLED_1sMode);
    }
    arm_matrix_instance_f32 ParamRow,ParamColumn;//一样的参数构建行矩阵和列矩阵
    float_t Param[6]={y*y,z*z,x,y,z,1};
    arm_mat_init_f32(&ParamRow,1,6,Param);
    arm_mat_init_f32(&ParamColumn,6,1,Param);

    arm_matrix_instance_f32 XTmp;
    float_t XTmpParam[36]={0};
    arm_mat_init_f32(&XTmp,6,6,XTmpParam);
    arm_mat_mult_f32(&ParamColumn,&ParamRow,&XTmp);//之后迭代累计

    arm_matrix_instance_f32 YTmp;
    float_t YTmpParam[6]={0};
    arm_mat_init_f32(&YTmp,6,1,YTmpParam);
    arm_mat_scale_f32(&ParamColumn,-x*x,&YTmp);//之后迭代累计

    arm_matrix_instance_f32 X;
    static float_t XParam[36]={0};
    arm_mat_init_f32(&X,6,6,XParam);
    
    arm_matrix_instance_f32 Y;
    static float_t YParam[6]={0};
    arm_mat_init_f32(&Y,6,1,YParam);

    //迭代计算均值
    for(uint8_t j=0;j<36;j++)
    {
        XParam[j]=Recursion_Mean(XParam[j],XTmpParam[j],i+1);
    }

    for(uint8_t j=0;j<6;j++)
    {
        YParam[j]=Recursion_Mean(YParam[j],YTmpParam[j],i+1);
    }
    

    if(i==6)
    {
        arm_mat_inverse_f32(&X,&X);//求个逆

        arm_matrix_instance_f32 GenMat;//椭球方程一般式矩阵
        float_t GenParam[6]={0};//椭球方程一般式参数
        arm_mat_init_f32(&GenMat,6,1,GenParam);

        arm_mat_mult_f32(&Y,&X,&GenMat);

        CAL_EllipsoidParamStruct->X0=-GenParam[2]/2;
        CAL_EllipsoidParamStruct->Y0=-GenParam[3]/(2*GenParam[0]);
        CAL_EllipsoidParamStruct->Z0=-GenParam[4]/(2*GenParam[1]);
        CAL_EllipsoidParamStruct->rX=sqrt(CAL_EllipsoidParamStruct->X0*
                                          CAL_EllipsoidParamStruct->X0+
                                          GenParam[0]*
                                          CAL_EllipsoidParamStruct->Y0*
                                          CAL_EllipsoidParamStruct->Y0+
                                          GenParam[1]*
                                          CAL_EllipsoidParamStruct->Z0*
                                          CAL_EllipsoidParamStruct->Z0-
                                          GenParam[5]);
        CAL_EllipsoidParamStruct->rY=CAL_EllipsoidParamStruct->rX*
                                     Fast_InvSqrt(CAL_EllipsoidParamStruct->rX);
        CAL_EllipsoidParamStruct->rZ=CAL_EllipsoidParamStruct->rX*
                                     Fast_InvSqrt(CAL_EllipsoidParamStruct->rY);
    }
    for (uint8_t j = 0; j < 10; j++)
    {
        SysTick_DelayMs(1);
        PPM_GetRCData(&ANO_DT_SendRCDataStruct);
    }
    while (ANO_DT_SendRCDataStruct.ANO_DT_RCAUX1<1100)
    {
        PPM_GetRCData(&ANO_DT_SendRCDataStruct);
        RGBLED_StateSet(i,RGBLED_1sMode);
    }
}