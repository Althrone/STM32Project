# STM32Project
针对stm32f407进行飞控开发。Editor: VSCode, Compiler: arm-none-eabi-gcc, Programmer: OpenOCD, Debugger: OpenOCD. STM32 open source development method.

工具链：Windows-VSCode-arm-none-eabi-gcc-make-OpenOCD

这个工程最主要的目的是对stm32芯片进行轻量化开发，现在多数初学者开发单片机都是不同芯片装不同IDE，体积臃肿，而且工作后如果从事这一行有可能被查水表。使用该方法可以使得程序全部在VSCode上编辑，只需要安装不同的编译器、烧录器和调试器即可。

VSCode不说了，最强IDE(雾。

arm-none-eabi-gcc其实就是一个开源的编译器，用这个编译器c程序在arm芯片上进行裸机开发。[GNU Arm Embedded Toolchain Downloads](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)`←鼠标中键点击以打开新窗口`

OpenOCD是一个开源的片上调试器，支持非常多芯片，同时也有烧录功能，这个工程的程序烧录也是靠它做的。

## 文件树
├─.vscode（vscode的一些设置）  
├─build（编译后的.o文件和二进制文件都放这里）  
├─source（源码所在）  
│  ├─CONTROL（飞控解算的程序）  
│  │  ├─inc  
│  │  └─src  
│  ├─CORE（407内核文件）  
│  ├─DEVICE（板载外设）  
│  │  ├─inc  
│  │  └─src  
│  ├─DRIVER（设备驱动）  
│  │  ├─inc  
│  │  └─src  
│  ├─FWLIB（ST的标准外设库）  
│  │  ├─inc  
│  │  └─src  
│  ├─LIB（一些直接调用的库文件，目前没用）  
│  ├─MATH（数学类函数实现）  
│  │  ├─inc  
│  │  └─src  
│  └─USER（用户文件夹）  
└─其他文件

## 更新进度
|时间|更新内容|
|-|-|
|2020/9/17|6050，气压计，磁罗盘的原始数据都能获取。串口发送正常，使用匿名v6协议。|
|2020/9/20|PPM解码正常，我一开始还想着用过采样的方式对PPM解码，结果外部中断更好用。|

## BUG
|所在文件|解释|
|-|-|
|source/CONTROL/src/attitude.c|俯仰角和横滚角的接算还算正常，但是偏航角的数据误差很大。|
|source/DRIVER/src/ANO_DT.c|误码率很高，估计是我串口写得太垃圾了。|