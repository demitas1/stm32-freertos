##########################################################
# STM32F411CE FreeRTOS LED Blink Project Makefile
##########################################################

######################################
# Target
######################################
TARGET = led-blink

######################################
# Building variables
######################################
DEBUG = 1
OPT = -Og

#######################################
# Paths
#######################################
BUILD_DIR = build

# Dependencies path (absolute path in container, relative for host)
# In Docker: /stm32-deps, On host: ../../../stm32-deps
DEPS_DIR ?= /stm32-deps

######################################
# Source
######################################
C_SOURCES = \
Core/Src/main.c \
Core/Src/stm32f4xx_it.c \
Core/Src/stm32f4xx_hal_msp.c \
$(DEPS_DIR)/stm32f4xx_hal_driver/Src/stm32f4xx_hal.c \
$(DEPS_DIR)/stm32f4xx_hal_driver/Src/stm32f4xx_hal_cortex.c \
$(DEPS_DIR)/stm32f4xx_hal_driver/Src/stm32f4xx_hal_gpio.c \
$(DEPS_DIR)/stm32f4xx_hal_driver/Src/stm32f4xx_hal_rcc.c \
$(DEPS_DIR)/stm32f4xx_hal_driver/Src/stm32f4xx_hal_rcc_ex.c \
$(DEPS_DIR)/stm32f4xx_hal_driver/Src/stm32f4xx_hal_pwr.c \
$(DEPS_DIR)/stm32f4xx_hal_driver/Src/stm32f4xx_hal_pwr_ex.c \
$(DEPS_DIR)/cmsis_device_f4/Source/Templates/system_stm32f4xx.c \
$(DEPS_DIR)/FreeRTOS-Kernel/croutine.c \
$(DEPS_DIR)/FreeRTOS-Kernel/event_groups.c \
$(DEPS_DIR)/FreeRTOS-Kernel/list.c \
$(DEPS_DIR)/FreeRTOS-Kernel/queue.c \
$(DEPS_DIR)/FreeRTOS-Kernel/stream_buffer.c \
$(DEPS_DIR)/FreeRTOS-Kernel/tasks.c \
$(DEPS_DIR)/FreeRTOS-Kernel/timers.c \
$(DEPS_DIR)/FreeRTOS-Kernel/portable/GCC/ARM_CM4F/port.c \
$(DEPS_DIR)/FreeRTOS-Kernel/portable/MemMang/heap_4.c

ASM_SOURCES = \
$(DEPS_DIR)/cmsis_device_f4/Source/Templates/gcc/startup_stm32f411xe.s

#######################################
# Binaries
#######################################
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#######################################
# CFLAGS
#######################################
CPU = -mcpu=cortex-m4
FPU = -mfpu=fpv4-sp-d16
FLOAT-ABI = -mfloat-abi=hard
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

AS_DEFS =

C_DEFS = \
-DUSE_HAL_DRIVER \
-DSTM32F411xE

AS_INCLUDES =

C_INCLUDES = \
-ICore/Inc \
-I$(DEPS_DIR)/stm32f4xx_hal_driver/Inc \
-I$(DEPS_DIR)/stm32f4xx_hal_driver/Inc/Legacy \
-I$(DEPS_DIR)/cmsis_device_f4/Include \
-I$(DEPS_DIR)/CMSIS_5/CMSIS/Core/Include \
-I$(DEPS_DIR)/FreeRTOS-Kernel/include \
-I$(DEPS_DIR)/FreeRTOS-Kernel/portable/GCC/ARM_CM4F

ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

#######################################
# LDFLAGS
#######################################
LDSCRIPT = STM32F411CEUx_FLASH.ld

LIBS = -lc -lm -lnosys
LIBDIR =
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

#######################################
# Build
#######################################
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(ASFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@

$(BUILD_DIR):
	mkdir $@

#######################################
# Flash
#######################################
flash: $(BUILD_DIR)/$(TARGET).bin
	st-flash write $< 0x8000000

flash-openocd: $(BUILD_DIR)/$(TARGET).elf
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg \
		-c "program $< verify reset exit"

#######################################
# Debug
#######################################
debug: $(BUILD_DIR)/$(TARGET).elf
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg &
	gdb-multiarch -ex "target remote :3333" -ex "monitor reset halt" $<

#######################################
# Clean
#######################################
clean:
	-rm -fR $(BUILD_DIR)

#######################################
# Dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

.PHONY: all clean flash flash-openocd debug
