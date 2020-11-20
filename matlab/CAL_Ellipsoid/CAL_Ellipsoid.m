clc;
close all;
clear all;

%读取磁力计三轴数据
wave_data=xlsread('D:\STM32Project\matlab\CAL_Ellipsoid\cal.csv');
FC_MAG_X=wave_data(3:end,7);
FC_MAG_Y=wave_data(3:end,8);
FC_MAG_Z=wave_data(3:end,9);

plot3(FC_MAG_X,FC_MAG_Y,FC_MAG_Z, '.');
% scatter3(FC_MAG_X,FC_MAG_Y,FC_MAG_Z,'k');

xlabel('x'),ylabel('y'),zlabel('z');
[VOX, VOY, VOZ, VRX, VRY, VRZ] = ellipsoidFit(FC_MAG_X, FC_MAG_Y, FC_MAG_Z);