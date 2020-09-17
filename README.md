# STM32Project
针对stm32f407进行飞控开发。Editor: VSCode, Compiler: arm-none-eabi-gcc, Programmer: OpenOCD, Debugger: OpenOCD. STM32 open source development method.

工具链：Windows-VSCode-arm-none-eabi-gcc-make-OpenOCD

这个工程最主要的目的是对stm32芯片进行轻量化开发，现在多数初学者开发单片机都是不同芯片装不同IDE，体积臃肿，而且工作后如果从事这一行有可能被查水表。使用该方法可以使得程序全部在VSCode上编辑，只需要安装不同的编译器、烧录器和调试器即可。

VSCode不说了，最强IDE(雾。

arm-none-eabi-gcc其实就是一个开源的编译器，用这个编译器c程序在arm芯片上进行裸机开发。[GNU Arm Embedded Toolchain Downloads](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)`←鼠标中键点击以打开新窗口`

OpenOCD是一个开源的片上调试器，支持非常多芯片，同时也有烧录功能，这个工程的程序烧录也是靠它做的。

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