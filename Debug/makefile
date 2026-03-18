################################################################################
# 自动生成的文件。不要编辑！
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# 此处定义了参与生成的所有源
-include sources.mk
-include my_drivers/User/Src/subdir.mk
-include my_drivers/LVGL/src/widgets/subdir.mk
-include my_drivers/LVGL/src/misc/subdir.mk
-include my_drivers/LVGL/src/hal/subdir.mk
-include my_drivers/LVGL/src/font/subdir.mk
-include my_drivers/LVGL/src/extra/widgets/win/subdir.mk
-include my_drivers/LVGL/src/extra/widgets/tileview/subdir.mk
-include my_drivers/LVGL/src/extra/widgets/tabview/subdir.mk
-include my_drivers/LVGL/src/extra/widgets/spinner/subdir.mk
-include my_drivers/LVGL/src/extra/widgets/spinbox/subdir.mk
-include my_drivers/LVGL/src/extra/widgets/span/subdir.mk
-include my_drivers/LVGL/src/extra/widgets/msgbox/subdir.mk
-include my_drivers/LVGL/src/extra/widgets/meter/subdir.mk
-include my_drivers/LVGL/src/extra/widgets/menu/subdir.mk
-include my_drivers/LVGL/src/extra/widgets/list/subdir.mk
-include my_drivers/LVGL/src/extra/widgets/led/subdir.mk
-include my_drivers/LVGL/src/extra/widgets/keyboard/subdir.mk
-include my_drivers/LVGL/src/extra/widgets/imgbtn/subdir.mk
-include my_drivers/LVGL/src/extra/widgets/colorwheel/subdir.mk
-include my_drivers/LVGL/src/extra/widgets/chart/subdir.mk
-include my_drivers/LVGL/src/extra/widgets/calendar/subdir.mk
-include my_drivers/LVGL/src/extra/widgets/animimg/subdir.mk
-include my_drivers/LVGL/src/extra/themes/mono/subdir.mk
-include my_drivers/LVGL/src/extra/themes/default/subdir.mk
-include my_drivers/LVGL/src/extra/themes/basic/subdir.mk
-include my_drivers/LVGL/src/extra/others/snapshot/subdir.mk
-include my_drivers/LVGL/src/extra/others/msg/subdir.mk
-include my_drivers/LVGL/src/extra/others/monkey/subdir.mk
-include my_drivers/LVGL/src/extra/others/imgfont/subdir.mk
-include my_drivers/LVGL/src/extra/others/ime/subdir.mk
-include my_drivers/LVGL/src/extra/others/gridnav/subdir.mk
-include my_drivers/LVGL/src/extra/others/fragment/subdir.mk
-include my_drivers/LVGL/src/extra/libs/sjpg/subdir.mk
-include my_drivers/LVGL/src/extra/libs/rlottie/subdir.mk
-include my_drivers/LVGL/src/extra/libs/qrcode/subdir.mk
-include my_drivers/LVGL/src/extra/libs/png/subdir.mk
-include my_drivers/LVGL/src/extra/libs/gif/subdir.mk
-include my_drivers/LVGL/src/extra/libs/fsdrv/subdir.mk
-include my_drivers/LVGL/src/extra/libs/freetype/subdir.mk
-include my_drivers/LVGL/src/extra/libs/ffmpeg/subdir.mk
-include my_drivers/LVGL/src/extra/libs/bmp/subdir.mk
-include my_drivers/LVGL/src/extra/layouts/grid/subdir.mk
-include my_drivers/LVGL/src/extra/layouts/flex/subdir.mk
-include my_drivers/LVGL/src/extra/subdir.mk
-include my_drivers/LVGL/src/draw/swm341_dma2d/subdir.mk
-include my_drivers/LVGL/src/draw/sw/subdir.mk
-include my_drivers/LVGL/src/draw/stm32_dma2d/subdir.mk
-include my_drivers/LVGL/src/draw/sdl/subdir.mk
-include my_drivers/LVGL/src/draw/nxp/vglite/subdir.mk
-include my_drivers/LVGL/src/draw/nxp/pxp/subdir.mk
-include my_drivers/LVGL/src/draw/nxp/subdir.mk
-include my_drivers/LVGL/src/draw/arm2d/subdir.mk
-include my_drivers/LVGL/src/draw/subdir.mk
-include my_drivers/LVGL/src/core/subdir.mk
-include my_drivers/LVGL/examples/porting/subdir.mk
-include my_drivers/LVGL/demos/widgets/assets/subdir.mk
-include my_drivers/LVGL/demos/widgets/subdir.mk
-include my_drivers/LVGL/demos/stress/subdir.mk
-include my_drivers/LVGL/demos/music/assets/subdir.mk
-include my_drivers/LVGL/demos/music/subdir.mk
-include my_drivers/LVGL/demos/keypad_encoder/subdir.mk
-include my_drivers/LVGL/demos/benchmark/assets/subdir.mk
-include my_drivers/LVGL/demos/benchmark/subdir.mk
-include my_core/ui/subdir.mk
-include my_core/feetech_motor/src/subdir.mk
-include SCSLib/subdir.mk
-include FreeRtos/portable/subdir.mk
-include FreeRtos/subdir.mk
-include Drivers/STM32H7xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := feetech_servo_h750
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# 将这些工具调用的输入和输出添加到构建变量 
EXECUTABLES += \
feetech_servo_h750.elf \

MAP_FILES += \
feetech_servo_h750.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
feetech_servo_h750.list \


# 所有目标
all: main-build

# 主构建目标
main-build: feetech_servo_h750.elf secondary-outputs

# 工具调用
feetech_servo_h750.elf feetech_servo_h750.map: $(OBJS) $(USER_OBJS) /home/ouo/li_lvgl/lv_motor/feetech_servo_h750/STM32H750XBHX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "feetech_servo_h750.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/STM32H750XBHX_FLASH.ld" --specs=nosys.specs -Wl,-Map="feetech_servo_h750.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo '已结束构建目标： $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo '已结束构建： $@'
	@echo ' '

feetech_servo_h750.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "feetech_servo_h750.list"
	@echo '已结束构建： $@'
	@echo ' '

# 其他目标
clean:
	-$(RM) default.size.stdout feetech_servo_h750.elf feetech_servo_h750.list feetech_servo_h750.map
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
