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
    while (ANO_DT_SendRCDataStruct.ANO_DT_RCAUX1>1900)
    {
        PPM_GetRCData(&ANO_DT_SendRCDataStruct);
        RGBLED_StateSet(RGBLED_Red,RGBLED_1sMode);
    }
    if (ANO_DT_SendRCDataStruct.ANO_DT_RCAUX1<1100)//获取陀螺仪零偏，地磁矢量
    {
        MPU6050_GyroCal();
        AK8975_GetGeomagneticVector();
        while (1)
        {
            // RGBLED_StateSet(RGBLED_Blue,RGBLED_1sMode);//矫正完成，显示绿色
        }
        
    }

    

    //先做椭球校准，加速度计和磁力计一起做，一共十二参数
    //飞机摆平在地面静置，测量地磁矢量和陀螺仪零偏
}

/**
 * @brief   通用型椭球校正
 * @param   x: 机体坐标系下传感器指向机头的分量
 * @param   y: 机体坐标系下传感器指向机身右侧的分量
 * @param   z: 机体坐标系下传感器指向机身下方的分量
 * 
 **/
void CAL_Ellipsoid(float_t x,float_t y,float_t z)
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
    arm_matrix_instance_f32 ParamRow,ParamColumn;//一样的参数构建行矩阵和列矩阵
    float_t Param[6]={y*y,z*z,x,y,z,1};
    arm_mat_init_f32(&ParamRow,1,6,Param);
    arm_mat_init_f32(&ParamColumn,6,1,Param);

    arm_matrix_instance_f32 X;
    float_t XParam[36]={0};
    arm_mat_init_f32(&X,6,6,XParam);
    arm_mat_mult_f32(&ParamColumn,&ParamRow,&X);
    arm_mat_inverse_f32(&X,&X);//求个逆

    arm_matrix_instance_f32 Y;
    float_t YParam[6]={0};
    arm_mat_init_f32(&Y,6,1,YParam);
    arm_mat_scale_f32(&ParamColumn,-x*x,&Y);

    arm_matrix_instance_f32 GenMat;//椭球方程一般式矩阵
    float_t GenParam[6]={0};//椭球方程一般式参数
    arm_mat_init_f32(&GenMat,6,1,GenParam);

    arm_mat_mult_f32(&Y,&X,&GenMat);
}