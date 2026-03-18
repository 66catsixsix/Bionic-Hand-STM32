################################################################################
# 自动生成的文件。不要编辑！
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
../my_drivers/LVGL/src/extra/widgets/chart/lv_chart.c 

OBJS += \
./my_drivers/LVGL/src/extra/widgets/chart/lv_chart.o 

C_DEPS += \
./my_drivers/LVGL/src/extra/widgets/chart/lv_chart.d 


# 每个子目录必须为构建它所贡献的源提供规则
my_drivers/LVGL/src/extra/widgets/chart/%.o my_drivers/LVGL/src/extra/widgets/chart/%.su my_drivers/LVGL/src/extra/widgets/chart/%.cyclo: ../my_drivers/LVGL/src/extra/widgets/chart/%.c my_drivers/LVGL/src/extra/widgets/chart/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../SCSLib -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/src -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/examples/porting -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/User/Inc -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/demos/music -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/demos/benchmark -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/FreeRtos/portable -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/FreeRtos/include -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_core/ui_H -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_core/feetech_motor/inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-my_drivers-2f-LVGL-2f-src-2f-extra-2f-widgets-2f-chart

clean-my_drivers-2f-LVGL-2f-src-2f-extra-2f-widgets-2f-chart:
	-$(RM) ./my_drivers/LVGL/src/extra/widgets/chart/lv_chart.cyclo ./my_drivers/LVGL/src/extra/widgets/chart/lv_chart.d ./my_drivers/LVGL/src/extra/widgets/chart/lv_chart.o ./my_drivers/LVGL/src/extra/widgets/chart/lv_chart.su

.PHONY: clean-my_drivers-2f-LVGL-2f-src-2f-extra-2f-widgets-2f-chart

