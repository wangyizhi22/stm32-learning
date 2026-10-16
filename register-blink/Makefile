# ============================================================
#  STM32F103C8T6 蓝板 Makefile
#  用法：  make          编译
#          make flash    编译并烧录
#          make clean    清理
# ============================================================

TARGET    = stm32-blink
BUILD_DIR = build

# ---- 工具链位置 ----
# 换机器时只要改这一行
TOOLCHAIN ?= $(HOME)/stm32-tools/xpack-arm-none-eabi-gcc-15.2.1-1.1/bin
PREFIX     = $(TOOLCHAIN)/arm-none-eabi-
CC         = $(PREFIX)gcc
AS         = $(PREFIX)gcc -x assembler-with-cpp
OBJCOPY    = $(PREFIX)objcopy
SIZE       = $(PREFIX)size

# ---- 目标芯片 ----
CPU  = -mcpu=cortex-m3 -mthumb
# USE_HAL_DRIVER 是通知 HAL 头文件「按 HAL 方式工作」的开关，不加会编译报错
DEFS = -DSTM32F103xB -DUSE_HAL_DRIVER

# ---- 源文件 ----
# HAL 驱动一共 60 多个 .c，这里只挑用得到的模块编译。
# 以后要用新外设（比如 SPI 接屏幕），把对应的 stm32f1xx_hal_xxx.c 补进 HAL_SOURCES 就行。
HAL_DIR     = Drivers/STM32F1xx_HAL_Driver/Src
HAL_SOURCES = $(HAL_DIR)/stm32f1xx_hal.c \
              $(HAL_DIR)/stm32f1xx_hal_cortex.c \
              $(HAL_DIR)/stm32f1xx_hal_rcc.c \
              $(HAL_DIR)/stm32f1xx_hal_rcc_ex.c \
              $(HAL_DIR)/stm32f1xx_hal_gpio.c \
              $(HAL_DIR)/stm32f1xx_hal_gpio_ex.c \
              $(HAL_DIR)/stm32f1xx_hal_exti.c \
              $(HAL_DIR)/stm32f1xx_hal_dma.c \
              $(HAL_DIR)/stm32f1xx_hal_flash.c \
              $(HAL_DIR)/stm32f1xx_hal_flash_ex.c \
              $(HAL_DIR)/stm32f1xx_hal_pwr.c \
              $(HAL_DIR)/stm32f1xx_hal_tim.c \
              $(HAL_DIR)/stm32f1xx_hal_tim_ex.c \
              $(HAL_DIR)/stm32f1xx_hal_uart.c \
              $(HAL_DIR)/stm32f1xx_hal_usart.c \
              $(HAL_DIR)/stm32f1xx_hal_spi.c \
              $(HAL_DIR)/stm32f1xx_hal_i2c.c \
              $(HAL_DIR)/stm32f1xx_hal_adc.c \
              $(HAL_DIR)/stm32f1xx_hal_dac.c \
              $(HAL_DIR)/stm32f1xx_hal_rtc.c \
              $(HAL_DIR)/stm32f1xx_hal_rtc_ex.c \
              $(HAL_DIR)/stm32f1xx_hal_crc.c \
              $(HAL_DIR)/stm32f1xx_hal_iwdg.c \
              $(HAL_DIR)/stm32f1xx_hal_wwdg.c

C_SOURCES   = src/main.c \
              src/syscalls.c \
              $(HAL_SOURCES) \
              Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.c
ASM_SOURCES = startup/startup_stm32f103xb.s

# ---- 头文件搜索路径 ----
C_INCLUDES = -Isrc \
             -IDrivers/CMSIS/Device/ST/STM32F1xx/Include \
             -IDrivers/CMSIS/Core/Include \
             -IDrivers/STM32F1xx_HAL_Driver/Inc

# ---- 编译选项 ----
# -Og  适合调试的优化等级（比 -O0 更好用，能看清变量）
# -g3 生成完整调试信息，断点、看变量全靠它
OPT     = -Og -g3
CFLAGS  = $(CPU) $(DEFS) $(C_INCLUDES) $(OPT) -Wall -Wextra \
          -ffunction-sections -fdata-sections
ASFLAGS = $(CPU) $(OPT) -Wall

# ---- 链接选项 ----
LDSCRIPT = linker/STM32F103XB_FLASH.ld
# 注意：这里故意不加 --specs=nosys.specs。
# 那些系统调用的空实现由 src/syscalls.c 提供，两套一起上会触发
# "_read is not implemented and will always fail" 之类的链接警告。
LDFLAGS  = $(CPU) -T$(LDSCRIPT) \
           --specs=nano.specs \
           -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref \
           -Wl,--gc-sections \
           -Wl,--no-warn-rwx-segments

# ---- 目标文件 ----
OBJECTS  = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))

vpath %.c $(sort $(dir $(C_SOURCES)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

# ============================================================
#  规则
# ============================================================

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -MMD -MP -MF $@.d $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(ASFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SIZE) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf
	$(OBJCOPY) -O ihex $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf
	$(OBJCOPY) -O binary -S $< $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)

# ---- 烧录 / 调试 ----
OPENOCD_DIR     = $(HOME)/stm32-tools/xpack-openocd-0.12.0-7
OPENOCD         = $(OPENOCD_DIR)/bin/openocd
OPENOCD_SCRIPTS = $(OPENOCD_DIR)/openocd/scripts

flash: $(BUILD_DIR)/$(TARGET).elf
	$(OPENOCD) -s $(OPENOCD_SCRIPTS) \
	  -f interface/stlink.cfg -f target/stm32f1x.cfg \
	  -c "program $< verify reset exit"

# 只检查能不能连上芯片，不烧录
probe:
	$(OPENOCD) -s $(OPENOCD_SCRIPTS) \
	  -f interface/stlink.cfg -f target/stm32f1x.cfg \
	  -c "init; reset halt; flash probe 0; exit"

.PHONY: all clean flash probe

-include $(wildcard $(BUILD_DIR)/*.d)
