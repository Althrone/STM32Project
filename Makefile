#用于编译FWLIB
#路径都是相对于/d/STM32Project的

NAME = IDCC
DATE = `date +%Y_%m_%d`

# c编译生成文件夹
BUILD_DIR = build

# C源文件
C_SOURCES =  \

# ASM sources
ASM_SOURCES =  \
source/FWLIB/startup_stm32f40_41xxx.s \

######################################
# building variables
######################################
# optimization
OPT = -Og

#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
CP = $(PREFIX)objcopy

#######################################
# CFLAGS
#######################################
# 这几个值请查阅arm-none-eabi-gcc的readme.txt
# 路径在你安装的文件夹的share\doc\gcc-arm-none-eabi
# cpu
CPU = -mcpu=cortex-m4
# ARCH = -march=armv7e-m

# fpu
FPU = -mfpu=fpv4-sp-d16

# float-abi
# FLOAT-ABI = -mfloat-abi=softfp
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = -mthumb $(CPU) $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = \

# C defines
# USE_STDPERIPH_DRIVER用于使用标准外设库，包含stm32f4xx_conf.h文件
# __VFP_FP__用于启动407的浮点核
C_DEFS =  \
-DUSE_STDPERIPH_DRIVER \
 
# AS includes
AS_INCLUDES = \
 
# C includes
C_INCLUDES =  \
-I

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
 
CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections


#######################################
#OpenOCD
#######################################
OCD_LINK_FILE = stlink-v2.cfg	#烧录器配置文件，用于普通买到的烧录器
# OCD_LINK_FILE = stlink-v2-1.cfg	#烧录器配置文件，用于stm32f4discovery
OCD_CHIP_FILE = stm32f4x.cfg	#芯片配置文件

#######################################
# 开始编译
# 先编译startup文件
#######################################

.PHONY: all startup core burn clean macro pwd

all: \
$(BUILD_DIR)/$(NAME)_$(DATE).bin \
$(BUILD_DIR)/$(NAME)_$(DATE).elf \
$(BUILD_DIR)/$(NAME)_$(DATE).hex

$(BUILD_DIR)/$(NAME)_$(DATE).bin:
	@rm -rf $(BUILD_DIR)/*.bin

# $(BUILD_DIR)/$(NAME)_$(DATE).elf:
# 	rm -rf $(BUILD_DIR)/*.elf

# $(BUILD_DIR)/$(NAME)_$(DATE).hex:
# 	rm -rf $(BUILD_DIR)/*.hex

startup: $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))
$(BUILD_DIR)/%.o: %.s
	$(CC) -c $(CFLAGS) -g $< -o $@

core:
	@echo Attention:build the core

burn:
	openocd \
	-f interface/$(OCD_LINK_FILE) \
	-f target/$(OCD_CHIP_FILE) \
	-c init \
	-c "reset halt" \
	-c "flash write_image erase $(wildcard $(BUILD_DIR)/*.bin)"

clean:
	rm -rf $(BUILD_DIR)/*

macro:
	touch gnuc_macro.c
	-rm gnuc_macro.h
	$(CC) -E -dM $(FLOAT-ABI) -c gnuc_macro.c >> gnuc_macro.h

pwd:
# @echo $(wildcard source/FWLIB/src/*.c)
	@echo $(words $(wildcard $(BUILD_DIR)/*.bin))
	@echo $(NAME)_$(DATE).bin