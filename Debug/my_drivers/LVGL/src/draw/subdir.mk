################################################################################
# 自动生成的文件。不要编辑！
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
../my_drivers/LVGL/src/draw/lv_draw.c \
../my_drivers/LVGL/src/draw/lv_draw_arc.c \
../my_drivers/LVGL/src/draw/lv_draw_img.c \
../my_drivers/LVGL/src/draw/lv_draw_label.c \
../my_drivers/LVGL/src/draw/lv_draw_layer.c \
../my_drivers/LVGL/src/draw/lv_draw_line.c \
../my_drivers/LVGL/src/draw/lv_draw_mask.c \
../my_drivers/LVGL/src/draw/lv_draw_rect.c \
../my_drivers/LVGL/src/draw/lv_draw_transform.c \
../my_drivers/LVGL/src/draw/lv_draw_triangle.c \
../my_drivers/LVGL/src/draw/lv_img_buf.c \
../my_drivers/LVGL/src/draw/lv_img_cache.c \
../my_drivers/LVGL/src/draw/lv_img_decoder.c 

OBJS += \
./my_drivers/LVGL/src/draw/lv_draw.o \
./my_drivers/LVGL/src/draw/lv_draw_arc.o \
./my_drivers/LVGL/src/draw/lv_draw_img.o \
./my_drivers/LVGL/src/draw/lv_draw_label.o \
./my_drivers/LVGL/src/draw/lv_draw_layer.o \
./my_drivers/LVGL/src/draw/lv_draw_line.o \
./my_drivers/LVGL/src/draw/lv_draw_mask.o \
./my_drivers/LVGL/src/draw/lv_draw_rect.o \
./my_drivers/LVGL/src/draw/lv_draw_transform.o \
./my_drivers/LVGL/src/draw/lv_draw_triangle.o \
./my_drivers/LVGL/src/draw/lv_img_buf.o \
./my_drivers/LVGL/src/draw/lv_img_cache.o \
./my_drivers/LVGL/src/draw/lv_img_decoder.o 

C_DEPS += \
./my_drivers/LVGL/src/draw/lv_draw.d \
./my_drivers/LVGL/src/draw/lv_draw_arc.d \
./my_drivers/LVGL/src/draw/lv_draw_img.d \
./my_drivers/LVGL/src/draw/lv_draw_label.d \
./my_drivers/LVGL/src/draw/lv_draw_layer.d \
./my_drivers/LVGL/src/draw/lv_draw_line.d \
./my_drivers/LVGL/src/draw/lv_draw_mask.d \
./my_drivers/LVGL/src/draw/lv_draw_rect.d \
./my_drivers/LVGL/src/draw/lv_draw_transform.d \
./my_drivers/LVGL/src/draw/lv_draw_triangle.d \
./my_drivers/LVGL/src/draw/lv_img_buf.d \
./my_drivers/LVGL/src/draw/lv_img_cache.d \
./my_drivers/LVGL/src/draw/lv_img_decoder.d 


# 每个子目录必须为构建它所贡献的源提供规则
my_drivers/LVGL/src/draw/%.o my_drivers/LVGL/src/draw/%.su my_drivers/LVGL/src/draw/%.cyclo: ../my_drivers/LVGL/src/draw/%.c my_drivers/LVGL/src/draw/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../SCSLib -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/src -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/examples/porting -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/User/Inc -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/demos/music -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/demos/benchmark -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/FreeRtos/portable -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/FreeRtos/include -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_core/ui_H -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_core/feetech_motor/inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-my_drivers-2f-LVGL-2f-src-2f-draw

clean-my_drivers-2f-LVGL-2f-src-2f-draw:
	-$(RM) ./my_drivers/LVGL/src/draw/lv_draw.cyclo ./my_drivers/LVGL/src/draw/lv_draw.d ./my_drivers/LVGL/src/draw/lv_draw.o ./my_drivers/LVGL/src/draw/lv_draw.su ./my_drivers/LVGL/src/draw/lv_draw_arc.cyclo ./my_drivers/LVGL/src/draw/lv_draw_arc.d ./my_drivers/LVGL/src/draw/lv_draw_arc.o ./my_drivers/LVGL/src/draw/lv_draw_arc.su ./my_drivers/LVGL/src/draw/lv_draw_img.cyclo ./my_drivers/LVGL/src/draw/lv_draw_img.d ./my_drivers/LVGL/src/draw/lv_draw_img.o ./my_drivers/LVGL/src/draw/lv_draw_img.su ./my_drivers/LVGL/src/draw/lv_draw_label.cyclo ./my_drivers/LVGL/src/draw/lv_draw_label.d ./my_drivers/LVGL/src/draw/lv_draw_label.o ./my_drivers/LVGL/src/draw/lv_draw_label.su ./my_drivers/LVGL/src/draw/lv_draw_layer.cyclo ./my_drivers/LVGL/src/draw/lv_draw_layer.d ./my_drivers/LVGL/src/draw/lv_draw_layer.o ./my_drivers/LVGL/src/draw/lv_draw_layer.su ./my_drivers/LVGL/src/draw/lv_draw_line.cyclo ./my_drivers/LVGL/src/draw/lv_draw_line.d ./my_drivers/LVGL/src/draw/lv_draw_line.o ./my_drivers/LVGL/src/draw/lv_draw_line.su ./my_drivers/LVGL/src/draw/lv_draw_mask.cyclo ./my_drivers/LVGL/src/draw/lv_draw_mask.d ./my_drivers/LVGL/src/draw/lv_draw_mask.o ./my_drivers/LVGL/src/draw/lv_draw_mask.su ./my_drivers/LVGL/src/draw/lv_draw_rect.cyclo ./my_drivers/LVGL/src/draw/lv_draw_rect.d ./my_drivers/LVGL/src/draw/lv_draw_rect.o ./my_drivers/LVGL/src/draw/lv_draw_rect.su ./my_drivers/LVGL/src/draw/lv_draw_transform.cyclo ./my_drivers/LVGL/src/draw/lv_draw_transform.d ./my_drivers/LVGL/src/draw/lv_draw_transform.o ./my_drivers/LVGL/src/draw/lv_draw_transform.su ./my_drivers/LVGL/src/draw/lv_draw_triangle.cyclo ./my_drivers/LVGL/src/draw/lv_draw_triangle.d ./my_drivers/LVGL/src/draw/lv_draw_triangle.o ./my_drivers/LVGL/src/draw/lv_draw_triangle.su ./my_drivers/LVGL/src/draw/lv_img_buf.cyclo ./my_drivers/LVGL/src/draw/lv_img_buf.d ./my_drivers/LVGL/src/draw/lv_img_buf.o ./my_drivers/LVGL/src/draw/lv_img_buf.su ./my_drivers/LVGL/src/draw/lv_img_cache.cyclo ./my_drivers/LVGL/src/draw/lv_img_cache.d ./my_drivers/LVGL/src/draw/lv_img_cache.o ./my_drivers/LVGL/src/draw/lv_img_cache.su ./my_drivers/LVGL/src/draw/lv_img_decoder.cyclo ./my_drivers/LVGL/src/draw/lv_img_decoder.d ./my_drivers/LVGL/src/draw/lv_img_decoder.o ./my_drivers/LVGL/src/draw/lv_img_decoder.su

.PHONY: clean-my_drivers-2f-LVGL-2f-src-2f-draw

