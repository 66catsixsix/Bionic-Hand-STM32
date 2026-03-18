################################################################################
# 自动生成的文件。不要编辑！
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
../my_drivers/LVGL/src/draw/sw/lv_draw_sw.c \
../my_drivers/LVGL/src/draw/sw/lv_draw_sw_arc.c \
../my_drivers/LVGL/src/draw/sw/lv_draw_sw_blend.c \
../my_drivers/LVGL/src/draw/sw/lv_draw_sw_dither.c \
../my_drivers/LVGL/src/draw/sw/lv_draw_sw_gradient.c \
../my_drivers/LVGL/src/draw/sw/lv_draw_sw_img.c \
../my_drivers/LVGL/src/draw/sw/lv_draw_sw_layer.c \
../my_drivers/LVGL/src/draw/sw/lv_draw_sw_letter.c \
../my_drivers/LVGL/src/draw/sw/lv_draw_sw_line.c \
../my_drivers/LVGL/src/draw/sw/lv_draw_sw_polygon.c \
../my_drivers/LVGL/src/draw/sw/lv_draw_sw_rect.c \
../my_drivers/LVGL/src/draw/sw/lv_draw_sw_transform.c 

OBJS += \
./my_drivers/LVGL/src/draw/sw/lv_draw_sw.o \
./my_drivers/LVGL/src/draw/sw/lv_draw_sw_arc.o \
./my_drivers/LVGL/src/draw/sw/lv_draw_sw_blend.o \
./my_drivers/LVGL/src/draw/sw/lv_draw_sw_dither.o \
./my_drivers/LVGL/src/draw/sw/lv_draw_sw_gradient.o \
./my_drivers/LVGL/src/draw/sw/lv_draw_sw_img.o \
./my_drivers/LVGL/src/draw/sw/lv_draw_sw_layer.o \
./my_drivers/LVGL/src/draw/sw/lv_draw_sw_letter.o \
./my_drivers/LVGL/src/draw/sw/lv_draw_sw_line.o \
./my_drivers/LVGL/src/draw/sw/lv_draw_sw_polygon.o \
./my_drivers/LVGL/src/draw/sw/lv_draw_sw_rect.o \
./my_drivers/LVGL/src/draw/sw/lv_draw_sw_transform.o 

C_DEPS += \
./my_drivers/LVGL/src/draw/sw/lv_draw_sw.d \
./my_drivers/LVGL/src/draw/sw/lv_draw_sw_arc.d \
./my_drivers/LVGL/src/draw/sw/lv_draw_sw_blend.d \
./my_drivers/LVGL/src/draw/sw/lv_draw_sw_dither.d \
./my_drivers/LVGL/src/draw/sw/lv_draw_sw_gradient.d \
./my_drivers/LVGL/src/draw/sw/lv_draw_sw_img.d \
./my_drivers/LVGL/src/draw/sw/lv_draw_sw_layer.d \
./my_drivers/LVGL/src/draw/sw/lv_draw_sw_letter.d \
./my_drivers/LVGL/src/draw/sw/lv_draw_sw_line.d \
./my_drivers/LVGL/src/draw/sw/lv_draw_sw_polygon.d \
./my_drivers/LVGL/src/draw/sw/lv_draw_sw_rect.d \
./my_drivers/LVGL/src/draw/sw/lv_draw_sw_transform.d 


# 每个子目录必须为构建它所贡献的源提供规则
my_drivers/LVGL/src/draw/sw/%.o my_drivers/LVGL/src/draw/sw/%.su my_drivers/LVGL/src/draw/sw/%.cyclo: ../my_drivers/LVGL/src/draw/sw/%.c my_drivers/LVGL/src/draw/sw/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../SCSLib -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/src -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/examples/porting -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/User/Inc -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/demos/music -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/demos/benchmark -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/FreeRtos/portable -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/FreeRtos/include -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_core/ui_H -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_core/feetech_motor/inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-my_drivers-2f-LVGL-2f-src-2f-draw-2f-sw

clean-my_drivers-2f-LVGL-2f-src-2f-draw-2f-sw:
	-$(RM) ./my_drivers/LVGL/src/draw/sw/lv_draw_sw.cyclo ./my_drivers/LVGL/src/draw/sw/lv_draw_sw.d ./my_drivers/LVGL/src/draw/sw/lv_draw_sw.o ./my_drivers/LVGL/src/draw/sw/lv_draw_sw.su ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_arc.cyclo ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_arc.d ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_arc.o ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_arc.su ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_blend.cyclo ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_blend.d ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_blend.o ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_blend.su ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_dither.cyclo ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_dither.d ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_dither.o ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_dither.su ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_gradient.cyclo ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_gradient.d ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_gradient.o ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_gradient.su ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_img.cyclo ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_img.d ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_img.o ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_img.su ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_layer.cyclo ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_layer.d ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_layer.o ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_layer.su ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_letter.cyclo ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_letter.d ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_letter.o ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_letter.su ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_line.cyclo ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_line.d ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_line.o ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_line.su ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_polygon.cyclo ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_polygon.d ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_polygon.o ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_polygon.su ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_rect.cyclo ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_rect.d ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_rect.o ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_rect.su ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_transform.cyclo ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_transform.d ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_transform.o ./my_drivers/LVGL/src/draw/sw/lv_draw_sw_transform.su

.PHONY: clean-my_drivers-2f-LVGL-2f-src-2f-draw-2f-sw

