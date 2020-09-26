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
├─matlab（控制算法的仿真还有仿真数据）  
├─source（源码所在）  
│  ├─CONTROL（飞控解算的程序）   
│  ├─CORE（407内核文件）  
│  ├─DEVICE（板载外设）  
│  ├─DRIVER（设备驱动）  
│  ├─DSPLIB（一些直接调用的库文件，主要是用来算矩阵的）  
│  ├─FWLIB（ST的标准外设库）  
│  ├─MATH（数学类函数实现）  
│  └─USER（用户文件夹）  
├─UnuesFWLIB（有一个标准库的c不支持407，我拉出来了，不然编译不了）  
└─其他文件

## 更新进度
|时间|更新内容|
|-|-|
|2020/9/17|6050，气压计，磁罗盘的原始数据都能获取。串口发送正常，使用匿名v6协议。|
|2020/9/20|PPM解码正常，我一开始还想着用过采样的方式对PPM解码，结果外部中断更好用。|
|2020/9/24|添加了arm的数学库，要用旧版本的，新版本不行，添加了一个matlab做EKF仿真的文件夹。|

## 遥控器各通道作用约定
|通道名称|通道作用|
|-|-|
|SWA|飞机上电前此拨杆打到2000，上电后飞机进入传感器校准模式，按照陀螺仪，加速度计，磁罗盘的顺序校准，校准完一个传感器之后要打到1000然后打回2000才能校准下一个传感器，校准完磁罗盘之后打到1000，退出校准模式。|

## BUG
|所在文件|解释|
|-|-|
|source/CONTROL/src/attitude.c|俯仰角和横滚角的接算还算正常，但是偏航角的数据误差很大。|
|source/DRIVER/src/ANO_DT.c|误码率很高，估计是我串口写得太垃圾了。|
|source/DSPLIB/arm_math.h|这个头文件我是从无名的代码那里拉过来的，我下最新版的头文件要添加两个莫名其妙的头文件，可以看这个文件夹内的unuse，而且添加完之后宏定义还和内核文件的宏定义有冲突。|