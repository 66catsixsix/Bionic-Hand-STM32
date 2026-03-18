################################################################################
# 自动生成的文件。不要编辑！
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
../my_drivers/LVGL/src/extra/widgets/animimg/lv_animimg.c 

OBJS += \
./my_drivers/LVGL/src/extra/widgets/animimg/lv_animimg.o 

C_DEPS += \
./my_drivers/LVGL/src/extra/widgets/animimg/lv_animimg.d 


# 每个子目录必须为构建它所贡献的源提供规则
my_drivers/LVGL/src/extra/widgets/animimg/%.o my_drivers/LVGL/src/extra/widgets/animimg/%.su my_drivers/LVGL/src/extra/widgets/animimg/%.cyclo: ../my_drivers/LVGL/src/extra/widgets/animimg/%.c my_drivers/LVGL/src/extra/widgets/animimg/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../SCSLib -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/src -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/examples/porting -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/User/Inc -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/demos/music -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/demos/benchmark -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/FreeRtos/portable -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/FreeRtos/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-my_drivers-2f-LVGL-2f-src-2f-extra-2f-widgets-2f-animimg

clean-my_drivers-2f-LVGL-2f-src-2f-extra-2f-widgets-2f-animimg:
	-$(RM) ./my_drivers/LVGL/src/extra/widgets/animimg/lv_animimg.cyclo ./my_drivers/LVGL/src/extra/widgets/animimg/lv_animimg.d ./my_drivers/LVGL/src/extra/widgets/animimg/lv_animimg.o ./my_drivers/LVGL/src/extra/widgets/animimg/lv_animimg.su

.PHONY: clean-my_drivers-2f-LVGL-2f-src-2f-extra-2f-widgets-2f-animimg

