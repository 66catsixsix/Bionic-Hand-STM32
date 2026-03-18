################################################################################
# 自动生成的文件。不要编辑！
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
../my_drivers/LVGL/src/extra/widgets/calendar/lv_calendar.c \
../my_drivers/LVGL/src/extra/widgets/calendar/lv_calendar_header_arrow.c \
../my_drivers/LVGL/src/extra/widgets/calendar/lv_calendar_header_dropdown.c 

OBJS += \
./my_drivers/LVGL/src/extra/widgets/calendar/lv_calendar.o \
./my_drivers/LVGL/src/extra/widgets/calendar/lv_calendar_header_arrow.o \
./my_drivers/LVGL/src/extra/widgets/calendar/lv_calendar_header_dropdown.o 

C_DEPS += \
./my_drivers/LVGL/src/extra/widgets/calendar/lv_calendar.d \
./my_drivers/LVGL/src/extra/widgets/calendar/lv_calendar_header_arrow.d \
./my_drivers/LVGL/src/extra/widgets/calendar/lv_calendar_header_dropdown.d 


# 每个子目录必须为构建它所贡献的源提供规则
my_drivers/LVGL/src/extra/widgets/calendar/%.o my_drivers/LVGL/src/extra/widgets/calendar/%.su my_drivers/LVGL/src/extra/widgets/calendar/%.cyclo: ../my_drivers/LVGL/src/extra/widgets/calendar/%.c my_drivers/LVGL/src/extra/widgets/calendar/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../SCSLib -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/src -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/examples/porting -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/User/Inc -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/demos/music -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/demos/benchmark -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/FreeRtos/portable -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/FreeRtos/include -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_core/ui_H -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_core/feetech_motor/inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-my_drivers-2f-LVGL-2f-src-2f-extra-2f-widgets-2f-calendar

clean-my_drivers-2f-LVGL-2f-src-2f-extra-2f-widgets-2f-calendar:
	-$(RM) ./my_drivers/LVGL/src/extra/widgets/calendar/lv_calendar.cyclo ./my_drivers/LVGL/src/extra/widgets/calendar/lv_calendar.d ./my_drivers/LVGL/src/extra/widgets/calendar/lv_calendar.o ./my_drivers/LVGL/src/extra/widgets/calendar/lv_calendar.su ./my_drivers/LVGL/src/extra/widgets/calendar/lv_calendar_header_arrow.cyclo ./my_drivers/LVGL/src/extra/widgets/calendar/lv_calendar_header_arrow.d ./my_drivers/LVGL/src/extra/widgets/calendar/lv_calendar_header_arrow.o ./my_drivers/LVGL/src/extra/widgets/calendar/lv_calendar_header_arrow.su ./my_drivers/LVGL/src/extra/widgets/calendar/lv_calendar_header_dropdown.cyclo ./my_drivers/LVGL/src/extra/widgets/calendar/lv_calendar_header_dropdown.d ./my_drivers/LVGL/src/extra/widgets/calendar/lv_calendar_header_dropdown.o ./my_drivers/LVGL/src/extra/widgets/calendar/lv_calendar_header_dropdown.su

.PHONY: clean-my_drivers-2f-LVGL-2f-src-2f-extra-2f-widgets-2f-calendar

