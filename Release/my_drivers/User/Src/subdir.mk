################################################################################
# 自动生成的文件。不要编辑！
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
../my_drivers/User/Src/lcd_rgb.c \
../my_drivers/User/Src/sdram.c \
../my_drivers/User/Src/touch_800x480.c \
../my_drivers/User/Src/touch_iic.c 

OBJS += \
./my_drivers/User/Src/lcd_rgb.o \
./my_drivers/User/Src/sdram.o \
./my_drivers/User/Src/touch_800x480.o \
./my_drivers/User/Src/touch_iic.o 

C_DEPS += \
./my_drivers/User/Src/lcd_rgb.d \
./my_drivers/User/Src/sdram.d \
./my_drivers/User/Src/touch_800x480.d \
./my_drivers/User/Src/touch_iic.d 


# 每个子目录必须为构建它所贡献的源提供规则
my_drivers/User/Src/%.o my_drivers/User/Src/%.su my_drivers/User/Src/%.cyclo: ../my_drivers/User/Src/%.c my_drivers/User/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../SCSLib -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/src -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/examples/porting -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/User/Inc -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/demos/music -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/demos/benchmark -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/FreeRtos/portable -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/FreeRtos/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-my_drivers-2f-User-2f-Src

clean-my_drivers-2f-User-2f-Src:
	-$(RM) ./my_drivers/User/Src/lcd_rgb.cyclo ./my_drivers/User/Src/lcd_rgb.d ./my_drivers/User/Src/lcd_rgb.o ./my_drivers/User/Src/lcd_rgb.su ./my_drivers/User/Src/sdram.cyclo ./my_drivers/User/Src/sdram.d ./my_drivers/User/Src/sdram.o ./my_drivers/User/Src/sdram.su ./my_drivers/User/Src/touch_800x480.cyclo ./my_drivers/User/Src/touch_800x480.d ./my_drivers/User/Src/touch_800x480.o ./my_drivers/User/Src/touch_800x480.su ./my_drivers/User/Src/touch_iic.cyclo ./my_drivers/User/Src/touch_iic.d ./my_drivers/User/Src/touch_iic.o ./my_drivers/User/Src/touch_iic.su

.PHONY: clean-my_drivers-2f-User-2f-Src

