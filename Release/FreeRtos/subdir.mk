################################################################################
# 自动生成的文件。不要编辑！
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
../FreeRtos/croutine.c \
../FreeRtos/event_groups.c \
../FreeRtos/list.c \
../FreeRtos/queue.c \
../FreeRtos/stream_buffer.c \
../FreeRtos/tasks.c \
../FreeRtos/timers.c 

OBJS += \
./FreeRtos/croutine.o \
./FreeRtos/event_groups.o \
./FreeRtos/list.o \
./FreeRtos/queue.o \
./FreeRtos/stream_buffer.o \
./FreeRtos/tasks.o \
./FreeRtos/timers.o 

C_DEPS += \
./FreeRtos/croutine.d \
./FreeRtos/event_groups.d \
./FreeRtos/list.d \
./FreeRtos/queue.d \
./FreeRtos/stream_buffer.d \
./FreeRtos/tasks.d \
./FreeRtos/timers.d 


# 每个子目录必须为构建它所贡献的源提供规则
FreeRtos/%.o FreeRtos/%.su FreeRtos/%.cyclo: ../FreeRtos/%.c FreeRtos/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../SCSLib -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/src -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/examples/porting -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/User/Inc -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/demos/music -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/demos/benchmark -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/FreeRtos/portable -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/FreeRtos/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-FreeRtos

clean-FreeRtos:
	-$(RM) ./FreeRtos/croutine.cyclo ./FreeRtos/croutine.d ./FreeRtos/croutine.o ./FreeRtos/croutine.su ./FreeRtos/event_groups.cyclo ./FreeRtos/event_groups.d ./FreeRtos/event_groups.o ./FreeRtos/event_groups.su ./FreeRtos/list.cyclo ./FreeRtos/list.d ./FreeRtos/list.o ./FreeRtos/list.su ./FreeRtos/queue.cyclo ./FreeRtos/queue.d ./FreeRtos/queue.o ./FreeRtos/queue.su ./FreeRtos/stream_buffer.cyclo ./FreeRtos/stream_buffer.d ./FreeRtos/stream_buffer.o ./FreeRtos/stream_buffer.su ./FreeRtos/tasks.cyclo ./FreeRtos/tasks.d ./FreeRtos/tasks.o ./FreeRtos/tasks.su ./FreeRtos/timers.cyclo ./FreeRtos/timers.d ./FreeRtos/timers.o ./FreeRtos/timers.su

.PHONY: clean-FreeRtos

