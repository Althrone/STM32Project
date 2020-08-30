#用于编译FWLIB
#路径都是相对于/d/STM32Project的

NAME = IDCC
DATE = `date +%Y_%m_%d`

# c编译生成文件夹
BUILD_DIR = build

# C源文件
STDPERIPH_C_SOURCES =  $(wildcard source/FWLIB/src/*.c)
DEVICE_C_SOURCES = $(wildcard source/DEVICE/src/*.c)
DRIVER_C_SOURCES = $(wildcard source/DRIVER/src/*.c)
USER_C_SOURCES = $(wildcard source/USER/*.c)

C_SOURCES = $(STDPERIPH_C_SOURCES) $(DEVICE_C_SOURCES) $(DRIVER_C_SOURCES) $(USER_C_SOURCES)


# ASM sources
ASM_SOURCES = \
source/CORE/startup_stm32f40_41xxx.s \

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
AS = $(PREFIX)gcc -x assembler-with-cpp
SZ = $(PREFIX)size

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
C_DEFS = \
-DUSE_STDPERIPH_DRIVER \
 
# AS includes
AS_INCLUDES = \
 
# C includes
C_INCLUDES = \
-Isource/USER \
-Isource/CORE \
-Isource/FWLIB/inc \
-Isource/DRIVER/inc \
-Isource/DEVICE/inc \


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
 
CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

#######################################
# LDFLAGS
#######################################
# link script  链接配置文件
LDSCRIPT = source/CORE/STM32F417IG_FLASH.ld

LDGROUP = -Wl,--start-group -lc -lm -Wl,--end-group

LDFLAGS = $(MCU) -T$(LDSCRIPT) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

#######################################
# SZFLAGS
#######################################


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
	# @rm -rf $(BUILD_DIR)/*.bin
	# @rm -rf $(BUILD_DIR)/*.hex
	# @rm -rf $(BUILD_DIR)/*.elf

STDPERIPH_OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(STDPERIPH_C_SOURCES:.c=.o)))

.PHONY: all startup core burn clean macro pwd

all: \
$(BUILD_DIR)/$(NAME)_$(DATE).elf

# $(BUILD_DIR)/$(NAME)_$(DATE).bin: $(BUILD_DIR)/$(NAME)_$(DATE).elf
# 	$(CP) -O binary $^ $@

# $(BUILD_DIR)/$(NAME)_$(DATE).hex: $(BUILD_DIR)/$(NAME)_$(DATE).elf
# 	$(CP) -O ihex $^ $@

$(BUILD_DIR)/$(NAME)_$(DATE).elf: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@
	$(SZ) $@

startup: $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))
$(BUILD_DIR)/%.o: %.s
	$(CC) -c $(CFLAGS) -g $< -o $@

core:
	@echo Attention:build the core

stdperiph:  $(STDPERIPH_OBJECTS)
	@echo aaaaaaaaaaaaaaaaaaaaa

$(STDPERIPH_OBJECTS): $(STDPERIPH_C_SOURCES)
	@echo Attention:build standard peripheral library
	$(CC) -c $(CFLAGS) $< -o $@

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
	@echo $(STDPERIPH_OBJECTS)
#@echo $(words $(wildcard $(BUILD_DIR)/*.bin))
#@echo $(NAME)_$(DATE).bin
#@echo $(BUILD_DIR)/$(NAME)_$(DATE).hex: $(BUILD_DIR)/$(NAME)_$(DATE).elf
#@echo $(STDPERIPH_C_SOURCES:.c=.o)
#@echo $(wildcard source/FWLIB/src/*.c)