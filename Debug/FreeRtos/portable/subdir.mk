################################################################################
# 自动生成的文件。不要编辑！
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
../FreeRtos/portable/heap_4.c \
../FreeRtos/portable/port.c 

OBJS += \
./FreeRtos/portable/heap_4.o \
./FreeRtos/portable/port.o 

C_DEPS += \
./FreeRtos/portable/heap_4.d \
./FreeRtos/portable/port.d 


# 每个子目录必须为构建它所贡献的源提供规则
FreeRtos/portable/%.o FreeRtos/portable/%.su FreeRtos/portable/%.cyclo: ../FreeRtos/portable/%.c FreeRtos/portable/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../SCSLib -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/src -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/examples/porting -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/User/Inc -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/demos/music -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/demos/benchmark -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/FreeRtos/portable -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/FreeRtos/include -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_core/ui_H -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_core/feetech_motor/inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-FreeRtos-2f-portable

clean-FreeRtos-2f-portable:
	-$(RM) ./FreeRtos/portable/heap_4.cyclo ./FreeRtos/portable/heap_4.d ./FreeRtos/portable/heap_4.o ./FreeRtos/portable/heap_4.su ./FreeRtos/portable/port.cyclo ./FreeRtos/portable/port.d ./FreeRtos/portable/port.o ./FreeRtos/portable/port.su

.PHONY: clean-FreeRtos-2f-portable

