################################################################################
# 自动生成的文件。不要编辑！
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
../my_drivers/LVGL/src/draw/sdl/lv_draw_sdl.c \
../my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_arc.c \
../my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_bg.c \
../my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_composite.c \
../my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_img.c \
../my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_label.c \
../my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_layer.c \
../my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_line.c \
../my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_mask.c \
../my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_polygon.c \
../my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_rect.c \
../my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_stack_blur.c \
../my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_texture_cache.c \
../my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_utils.c 

OBJS += \
./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl.o \
./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_arc.o \
./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_bg.o \
./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_composite.o \
./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_img.o \
./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_label.o \
./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_layer.o \
./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_line.o \
./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_mask.o \
./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_polygon.o \
./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_rect.o \
./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_stack_blur.o \
./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_texture_cache.o \
./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_utils.o 

C_DEPS += \
./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl.d \
./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_arc.d \
./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_bg.d \
./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_composite.d \
./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_img.d \
./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_label.d \
./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_layer.d \
./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_line.d \
./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_mask.d \
./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_polygon.d \
./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_rect.d \
./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_stack_blur.d \
./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_texture_cache.d \
./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_utils.d 


# 每个子目录必须为构建它所贡献的源提供规则
my_drivers/LVGL/src/draw/sdl/%.o my_drivers/LVGL/src/draw/sdl/%.su my_drivers/LVGL/src/draw/sdl/%.cyclo: ../my_drivers/LVGL/src/draw/sdl/%.c my_drivers/LVGL/src/draw/sdl/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../SCSLib -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/src -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/examples/porting -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/User/Inc -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/demos/music -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/demos/benchmark -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/FreeRtos/portable -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/FreeRtos/include -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_core/ui_H -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_core/feetech_motor/inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-my_drivers-2f-LVGL-2f-src-2f-draw-2f-sdl

clean-my_drivers-2f-LVGL-2f-src-2f-draw-2f-sdl:
	-$(RM) ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl.cyclo ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl.d ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl.o ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl.su ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_arc.cyclo ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_arc.d ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_arc.o ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_arc.su ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_bg.cyclo ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_bg.d ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_bg.o ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_bg.su ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_composite.cyclo ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_composite.d ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_composite.o ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_composite.su ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_img.cyclo ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_img.d ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_img.o ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_img.su ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_label.cyclo ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_label.d ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_label.o ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_label.su ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_layer.cyclo ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_layer.d ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_layer.o ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_layer.su ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_line.cyclo ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_line.d ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_line.o ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_line.su ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_mask.cyclo ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_mask.d ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_mask.o ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_mask.su ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_polygon.cyclo ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_polygon.d ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_polygon.o ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_polygon.su ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_rect.cyclo ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_rect.d ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_rect.o ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_rect.su ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_stack_blur.cyclo ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_stack_blur.d ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_stack_blur.o ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_stack_blur.su ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_texture_cache.cyclo ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_texture_cache.d ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_texture_cache.o ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_texture_cache.su ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_utils.cyclo ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_utils.d ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_utils.o ./my_drivers/LVGL/src/draw/sdl/lv_draw_sdl_utils.su

.PHONY: clean-my_drivers-2f-LVGL-2f-src-2f-draw-2f-sdl

