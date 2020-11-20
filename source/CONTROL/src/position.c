#include "position.h"

/**
 * 使用墨卡托投影地图作为无人机移动坐标，相当于无人机在一个正方形棋盘内移动。
 * 经度（x轴）范围是-π~π，x = π*(Θ/180°)经线等距分布，经度可以完全表示
 * 纬度（y轴）范围是-π~π，y = sign(Φ)*ln(tan(45° + abs(Φ/2))，纬度在-85°~85°之间
 **/

extern GPS_DataTypeDef GPS_DataStruct;//引入GPS数据的全局变量

Filter_CompParamTypeDef POS_AccCompParamStruct;//加速度环互补滤波参数
Filter_CompParamTypeDef POS_SpeedCompParamStruct;//速度环互补滤波参数

Filter_CompInfoTypeDef POS_AccXCompInfoStruct;//加速度环X轴数据
Filter_CompInfoTypeDef POS_SpeedXCompInfoStruct;//速度环X轴数据

Filter_CompInfoTypeDef POS_AccYCompInfoStruct;//加速度环Y轴数据
Filter_CompInfoTypeDef POS_SpeedYCompInfoStruct;//速度环Y轴数据

// #define PI  3.14159265358979323846f//math里面定义了

/**
 * @brief   无人机定点，三阶互补滤波
 * @param   GPS_DataStruct: GPS经纬度，海拔高度，定位精度
 * @param   MPU6050_CalDataStruct: 6050校正之后的值
 * @param   ATT_QuatDataTypeDef: 机体姿态四元素，用于将机体坐标系转至导航坐标系（东北天）
 **/
void POS_3OrderCompUpdate(GPS_DataTypeDef* GPS_DataStruct,
                          MPU6050_CalDataTypeDef* MPU6050_CalDataStruct,
                          ATT_QuatDataTypeDef* ATT_QuatDataStruct)
{
    //通过GPS计算无人机位移距离
    POS_DistTypeDef POS_DistStruct;
    POS_LongAndLat2Dist(GPS_DataStruct,&POS_DistStruct);

    //将机体坐标系的加速度量转化到导航坐标系(b->n)
    Mat_RotDataTypeDef nAccDataStruct;
    nAccDataStruct.Data1=MPU6050_CalDataStruct->MPU6050_CalAccelX;
    nAccDataStruct.Data2=MPU6050_CalDataStruct->MPU6050_CalAccelY;
    nAccDataStruct.Data3=MPU6050_CalDataStruct->MPU6050_CalAccelZ;
    Mat_TransRotQuat((Mat_RotQuatTypeDef*)&ATT_QuatDataStruct,
                     &nAccDataStruct);

    //x轴三阶互补
    //加速度环
    POS_AccXCompInfoStruct.Error=POS_DistStruct.X;
    POS_AccXCompInfoStruct.Input=nAccDataStruct.Data1;
    Filter_Comp(&POS_AccCompParamStruct,&POS_AccXCompInfoStruct);

    //速度环
    POS_SpeedXCompInfoStruct.Error=POS_DistStruct.X;
    POS_SpeedXCompInfoStruct.Input=POS_AccXCompInfoStruct.Output;
    Filter_Comp(&POS_SpeedCompParamStruct,&POS_SpeedXCompInfoStruct);

    //y轴三阶互补
    //加速度环
    POS_AccYCompInfoStruct.Error=POS_DistStruct.Y;
    POS_AccYCompInfoStruct.Input=nAccDataStruct.Data2;
    Filter_Comp(&POS_AccCompParamStruct,&POS_AccYCompInfoStruct);

    //速度环
    POS_SpeedYCompInfoStruct.Error=POS_DistStruct.Y;
    POS_SpeedYCompInfoStruct.Input=POS_AccYCompInfoStruct.Output;
    Filter_Comp(&POS_SpeedCompParamStruct,&POS_SpeedYCompInfoStruct);
}

/**
 * @brief   经纬度转距离
 * @param   GPS_DataStruct: GPS数据，主要是用经度纬度
 * @param   POS_DistStruct: 经纬度转化成平面上所移动的距离
 **/
void POS_LongAndLat2Dist(GPS_DataTypeDef* GPS_DataStruct,
                         POS_DistTypeDef* POS_DistStruct)
{
    //常量，地球半径
    const float_t EARTH_RADIUS = 6378137.0f;//单位m
    //静态变量，上一时刻经纬度
    static float_t prevLong;
    static float_t prevLat;
    //经纬度差值
    float_t diffLong,diffLat;
    float_t nowLong=GPS_DataStruct->GPS_LocationStruct.GPS_Longitude;
    float_t nowLat=GPS_DataStruct->GPS_LocationStruct.GPS_Latitude;
    //两点间距离
    float_t s;

    diffLong=nowLong-prevLong;
    diffLat=nowLat-prevLat;

    //弧长=2πr*Θ/360°
    //导航坐标系，y指向正北，x指向正东
    //先确定y方向走了多少
    POS_DistStruct->Y=2*PI*EARTH_RADIUS*diffLat/360.f;
    //然后计算两点之间的直线距离s
    s = EARTH_RADIUS * asinf(sinf(nowLat)*sinf(prevLat)*cosf(nowLong-prevLong)+cosf(nowLat)*cosf(prevLat));
    //因为不同纬度相同经度角走过的路程不一样，求个平均值x=√(s^2-y^2)
    if(diffLong<0.f)
        POS_DistStruct->X=-sqrtf(s*s-POS_DistStruct->Y*POS_DistStruct->Y);
    else if(diffLong>0.f)
        POS_DistStruct->X=sqrtf(s*s-POS_DistStruct->Y*POS_DistStruct->Y);
    else
        POS_DistStruct->X=0;

    prevLong=GPS_DataStruct->GPS_LocationStruct.GPS_Longitude;
    prevLat=GPS_DataStruct->GPS_LocationStruct.GPS_Latitude;   
}

/**
 * @brief   三阶互补滤波C(s)参数初始化
 **/
void POS_3OrderCompParamInit(void)
{
    POS_AccCompParamStruct.Kp=0;
    POS_AccCompParamStruct.Ki=0;

    POS_SpeedCompParamStruct.Kp=0;
    POS_SpeedCompParamStruct.Ki=0;
}