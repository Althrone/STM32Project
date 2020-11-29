clc;
close all;
clear all;

%读取加速度积分速度数据和气压计微分速度数据
wave_data=xlsread('D:\STM32Project\matlab\Fourier_Transform\ft.csv');
accSpeed=wave_data(3:end,14);
spl06Speed=wave_data(3:end,15);

fs=100;%采样频率100Hz
T=1/fs;%采样周期10ms一次
N=128;%数据点数
n=0:N-1;
t=n/fs;


y=fft(spl06Speed,N);
m=abs(y);
f=n*fs/N;
plot(f,m);

% plot(y,spl06Speed);