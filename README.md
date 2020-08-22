# STM32Project
针对stm32f407进行飞控开发。Editor: VSCode, Compiler: arm-none-eabi-gcc, Programmer: OpenOCD, Debugger: OpenOCD. STM32 open source development method.

工具链：Windows-VSCode-arm-none-eabi-gcc-make-OpenOCD

## 文件层次
|根文件夹|内容|
|-|-|
|source|所有.c和.h文件放在这里|
|build|编译过程的中间文件，最后的烧录文件放在这里|
|tools|非程序的工具|

|source|内容|
|-|-|
||stm32f4的标准库文件|
|user|用户自己编写的用于实现特定功能的程序|
|math|矩阵解算程序|
|filter|各种滤波器|
|dev|设备和各种硬件芯片或者模块的程序|
|drv|驱动，主要是各种协议|
|STM32F4xx_StdPeriph_Driver|标准库文件|