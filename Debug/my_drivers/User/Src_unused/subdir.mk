################################################################################
# 自动生成的文件。不要编辑！
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
../my_drivers/User/Src_unused/lcd_pwm.c \
../my_drivers/User/Src_unused/lcd_test.c \
../my_drivers/User/Src_unused/led.c \
../my_drivers/User/Src_unused/usart.c 

OBJS += \
./my_drivers/User/Src_unused/lcd_pwm.o \
./my_drivers/User/Src_unused/lcd_test.o \
./my_drivers/User/Src_unused/led.o \
./my_drivers/User/Src_unused/usart.o 

C_DEPS += \
./my_drivers/User/Src_unused/lcd_pwm.d \
./my_drivers/User/Src_unused/lcd_test.d \
./my_drivers/User/Src_unused/led.d \
./my_drivers/User/Src_unused/usart.d 


# 每个子目录必须为构建它所贡献的源提供规则
my_drivers/User/Src_unused/%.o my_drivers/User/Src_unused/%.su my_drivers/User/Src_unused/%.cyclo: ../my_drivers/User/Src_unused/%.c my_drivers/User/Src_unused/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../SCSLib -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/src -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/examples/porting -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/User/Inc -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/demos/music -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/demos/benchmark -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-my_drivers-2f-User-2f-Src_unused

clean-my_drivers-2f-User-2f-Src_unused:
	-$(RM) ./my_drivers/User/Src_unused/lcd_pwm.cyclo ./my_drivers/User/Src_unused/lcd_pwm.d ./my_drivers/User/Src_unused/lcd_pwm.o ./my_drivers/User/Src_unused/lcd_pwm.su ./my_drivers/User/Src_unused/lcd_test.cyclo ./my_drivers/User/Src_unused/lcd_test.d ./my_drivers/User/Src_unused/lcd_test.o ./my_drivers/User/Src_unused/lcd_test.su ./my_drivers/User/Src_unused/led.cyclo ./my_drivers/User/Src_unused/led.d ./my_drivers/User/Src_unused/led.o ./my_drivers/User/Src_unused/led.su ./my_drivers/User/Src_unused/usart.cyclo ./my_drivers/User/Src_unused/usart.d ./my_drivers/User/Src_unused/usart.o ./my_drivers/User/Src_unused/usart.su

.PHONY: clean-my_drivers-2f-User-2f-Src_unused

