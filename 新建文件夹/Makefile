# 工程文件夹
TARGET = STM32Project
 
# c编译生成文件夹
BUILD_DIR = build

# C源文件
C_SOURCES =  \
sourece/user/main.c \


# ASM sources
ASM_SOURCES =  \
source/FWLIB/startup_stm32f40_41xxx.s \
asdfasf.s

C_INCLUDES =  \
-Isource/FWLIB \
-Isource/FWLIB/inc \
-Isource/user


#######################################
# ARM交叉编译器变量
#######################################
ARM_PREFIX	= arm-none-eabi-
CC			= $(ARM_PREFIX)gcc
CP			= $(ARM_PREFIX)objcopy

#######################################
# CFLAGS
#######################################
CPU 		+= -mcpu=cortex-m4
CPUFLAGS	+= -mthumb	#指定指令集
 
# fpu
# NONE for Cortex-M0/M0+/M3

# float-abi


# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# C defines   宏定义标志
C_DEFS =  \
-DUSE_STDPERIPH_DRIVER \
-DSTM32F10X_HD

#######################################
# LDFLAGS
#######################################
# link script  链接配置文件
LD_FILE = \
source/FWLIB/STM32F417IG_FLASH.ld

#######################################
#OpenOCD
#######################################
OCD_LINK_FILE = stlink-v2.cfg	#烧录器配置文件，用于普通买到的烧录器
# OCD_LINK_FILE = stlink-v2-1.cfg	#烧录器配置文件，用于stm32f4discovery
OCD_CHIP_FILE = stm32f4x.cfg	#芯片配置文件

DATE = `date +%Y-%m-%d`


#######################################
# build the application
#######################################

vpath %.c $(sort $(dir $(C_SOURCES)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))


$(BUILD_DIR)/%.o: %.c
	$(CC) -c $(CPUFLAGS) -g -Wall $(INC_DIR) $^ -o $@

.PHONY: all burn date clean test core

all: build/system_stm32f4xx.d build/system_stm32f4xx.o

build/system_stm32f4xx.d: source/user/system_stm32f4xx.c
	@set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) -Isource/FWLIB $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

build/system_stm32f4xx.o: 



burn:
	openocd \
	-f interface/$(OCD_LINK_FILE) \
	-f target/$(OCD_CHIP_FILE) \
	-c init \
	-c "reset halt" \
	-c "flash write_image erase $(BUILD_DIR)/xxxxxxxxxxxxxxxxxxxxx.bin"

#######################################
#测试命令
#######################################
# VPATH = source/FWLIB
# vpath %.h source/FWLIB    //指定.h类型文件的搜索路径是source/FWLIB
test:
	$(CC) -MM source/user/system_stm32f4xx.c -Isource/FWLIB -MP -MF build/system_stm32f4xx.d
core:
	$(CC) -MM source/FWLIB/src/*.c %(C_INCLUDES) -MP -MF build/*.d
date:
	touch build/$(DATE).c

# 为了防止更改build目录的时候误删其他文件，rm -rf的路径不作更改
clean:
	@echo /////////////!!Attention!!/////////////
	@echo \\\\\\\\\\\\\\\\\\\\\\\\\\!!Attention!!\\\\\\\\\\\\\\\\\\\\\\\\\\
	@echo /////////////!!Attention!!/////////////
	rm -rf build/*

#######################################
# dependencies
#######################################
# -include $(wildcard $(BUILD_DIR)/*.d)

$(BUILD_DIR)/%.d: source/FWLIB/src/%.c
	@set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $(C_INCLUDES) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$