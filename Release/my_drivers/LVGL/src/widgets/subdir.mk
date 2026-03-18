################################################################################
# 自动生成的文件。不要编辑！
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
../my_drivers/LVGL/src/widgets/lv_arc.c \
../my_drivers/LVGL/src/widgets/lv_bar.c \
../my_drivers/LVGL/src/widgets/lv_btn.c \
../my_drivers/LVGL/src/widgets/lv_btnmatrix.c \
../my_drivers/LVGL/src/widgets/lv_canvas.c \
../my_drivers/LVGL/src/widgets/lv_checkbox.c \
../my_drivers/LVGL/src/widgets/lv_dropdown.c \
../my_drivers/LVGL/src/widgets/lv_img.c \
../my_drivers/LVGL/src/widgets/lv_label.c \
../my_drivers/LVGL/src/widgets/lv_line.c \
../my_drivers/LVGL/src/widgets/lv_objx_templ.c \
../my_drivers/LVGL/src/widgets/lv_roller.c \
../my_drivers/LVGL/src/widgets/lv_slider.c \
../my_drivers/LVGL/src/widgets/lv_switch.c \
../my_drivers/LVGL/src/widgets/lv_table.c \
../my_drivers/LVGL/src/widgets/lv_textarea.c 

OBJS += \
./my_drivers/LVGL/src/widgets/lv_arc.o \
./my_drivers/LVGL/src/widgets/lv_bar.o \
./my_drivers/LVGL/src/widgets/lv_btn.o \
./my_drivers/LVGL/src/widgets/lv_btnmatrix.o \
./my_drivers/LVGL/src/widgets/lv_canvas.o \
./my_drivers/LVGL/src/widgets/lv_checkbox.o \
./my_drivers/LVGL/src/widgets/lv_dropdown.o \
./my_drivers/LVGL/src/widgets/lv_img.o \
./my_drivers/LVGL/src/widgets/lv_label.o \
./my_drivers/LVGL/src/widgets/lv_line.o \
./my_drivers/LVGL/src/widgets/lv_objx_templ.o \
./my_drivers/LVGL/src/widgets/lv_roller.o \
./my_drivers/LVGL/src/widgets/lv_slider.o \
./my_drivers/LVGL/src/widgets/lv_switch.o \
./my_drivers/LVGL/src/widgets/lv_table.o \
./my_drivers/LVGL/src/widgets/lv_textarea.o 

C_DEPS += \
./my_drivers/LVGL/src/widgets/lv_arc.d \
./my_drivers/LVGL/src/widgets/lv_bar.d \
./my_drivers/LVGL/src/widgets/lv_btn.d \
./my_drivers/LVGL/src/widgets/lv_btnmatrix.d \
./my_drivers/LVGL/src/widgets/lv_canvas.d \
./my_drivers/LVGL/src/widgets/lv_checkbox.d \
./my_drivers/LVGL/src/widgets/lv_dropdown.d \
./my_drivers/LVGL/src/widgets/lv_img.d \
./my_drivers/LVGL/src/widgets/lv_label.d \
./my_drivers/LVGL/src/widgets/lv_line.d \
./my_drivers/LVGL/src/widgets/lv_objx_templ.d \
./my_drivers/LVGL/src/widgets/lv_roller.d \
./my_drivers/LVGL/src/widgets/lv_slider.d \
./my_drivers/LVGL/src/widgets/lv_switch.d \
./my_drivers/LVGL/src/widgets/lv_table.d \
./my_drivers/LVGL/src/widgets/lv_textarea.d 


# 每个子目录必须为构建它所贡献的源提供规则
my_drivers/LVGL/src/widgets/%.o my_drivers/LVGL/src/widgets/%.su my_drivers/LVGL/src/widgets/%.cyclo: ../my_drivers/LVGL/src/widgets/%.c my_drivers/LVGL/src/widgets/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../SCSLib -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/src -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/examples/porting -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/User/Inc -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/demos/music -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/demos/benchmark -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/FreeRtos/portable -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/FreeRtos/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-my_drivers-2f-LVGL-2f-src-2f-widgets

clean-my_drivers-2f-LVGL-2f-src-2f-widgets:
	-$(RM) ./my_drivers/LVGL/src/widgets/lv_arc.cyclo ./my_drivers/LVGL/src/widgets/lv_arc.d ./my_drivers/LVGL/src/widgets/lv_arc.o ./my_drivers/LVGL/src/widgets/lv_arc.su ./my_drivers/LVGL/src/widgets/lv_bar.cyclo ./my_drivers/LVGL/src/widgets/lv_bar.d ./my_drivers/LVGL/src/widgets/lv_bar.o ./my_drivers/LVGL/src/widgets/lv_bar.su ./my_drivers/LVGL/src/widgets/lv_btn.cyclo ./my_drivers/LVGL/src/widgets/lv_btn.d ./my_drivers/LVGL/src/widgets/lv_btn.o ./my_drivers/LVGL/src/widgets/lv_btn.su ./my_drivers/LVGL/src/widgets/lv_btnmatrix.cyclo ./my_drivers/LVGL/src/widgets/lv_btnmatrix.d ./my_drivers/LVGL/src/widgets/lv_btnmatrix.o ./my_drivers/LVGL/src/widgets/lv_btnmatrix.su ./my_drivers/LVGL/src/widgets/lv_canvas.cyclo ./my_drivers/LVGL/src/widgets/lv_canvas.d ./my_drivers/LVGL/src/widgets/lv_canvas.o ./my_drivers/LVGL/src/widgets/lv_canvas.su ./my_drivers/LVGL/src/widgets/lv_checkbox.cyclo ./my_drivers/LVGL/src/widgets/lv_checkbox.d ./my_drivers/LVGL/src/widgets/lv_checkbox.o ./my_drivers/LVGL/src/widgets/lv_checkbox.su ./my_drivers/LVGL/src/widgets/lv_dropdown.cyclo ./my_drivers/LVGL/src/widgets/lv_dropdown.d ./my_drivers/LVGL/src/widgets/lv_dropdown.o ./my_drivers/LVGL/src/widgets/lv_dropdown.su ./my_drivers/LVGL/src/widgets/lv_img.cyclo ./my_drivers/LVGL/src/widgets/lv_img.d ./my_drivers/LVGL/src/widgets/lv_img.o ./my_drivers/LVGL/src/widgets/lv_img.su ./my_drivers/LVGL/src/widgets/lv_label.cyclo ./my_drivers/LVGL/src/widgets/lv_label.d ./my_drivers/LVGL/src/widgets/lv_label.o ./my_drivers/LVGL/src/widgets/lv_label.su ./my_drivers/LVGL/src/widgets/lv_line.cyclo ./my_drivers/LVGL/src/widgets/lv_line.d ./my_drivers/LVGL/src/widgets/lv_line.o ./my_drivers/LVGL/src/widgets/lv_line.su ./my_drivers/LVGL/src/widgets/lv_objx_templ.cyclo ./my_drivers/LVGL/src/widgets/lv_objx_templ.d ./my_drivers/LVGL/src/widgets/lv_objx_templ.o ./my_drivers/LVGL/src/widgets/lv_objx_templ.su ./my_drivers/LVGL/src/widgets/lv_roller.cyclo ./my_drivers/LVGL/src/widgets/lv_roller.d ./my_drivers/LVGL/src/widgets/lv_roller.o ./my_drivers/LVGL/src/widgets/lv_roller.su ./my_drivers/LVGL/src/widgets/lv_slider.cyclo ./my_drivers/LVGL/src/widgets/lv_slider.d ./my_drivers/LVGL/src/widgets/lv_slider.o ./my_drivers/LVGL/src/widgets/lv_slider.su ./my_drivers/LVGL/src/widgets/lv_switch.cyclo ./my_drivers/LVGL/src/widgets/lv_switch.d ./my_drivers/LVGL/src/widgets/lv_switch.o ./my_drivers/LVGL/src/widgets/lv_switch.su ./my_drivers/LVGL/src/widgets/lv_table.cyclo ./my_drivers/LVGL/src/widgets/lv_table.d ./my_drivers/LVGL/src/widgets/lv_table.o ./my_drivers/LVGL/src/widgets/lv_table.su ./my_drivers/LVGL/src/widgets/lv_textarea.cyclo ./my_drivers/LVGL/src/widgets/lv_textarea.d ./my_drivers/LVGL/src/widgets/lv_textarea.o ./my_drivers/LVGL/src/widgets/lv_textarea.su

.PHONY: clean-my_drivers-2f-LVGL-2f-src-2f-widgets

