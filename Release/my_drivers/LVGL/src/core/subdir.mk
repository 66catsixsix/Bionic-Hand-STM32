################################################################################
# 自动生成的文件。不要编辑！
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
../my_drivers/LVGL/src/core/lv_disp.c \
../my_drivers/LVGL/src/core/lv_event.c \
../my_drivers/LVGL/src/core/lv_group.c \
../my_drivers/LVGL/src/core/lv_indev.c \
../my_drivers/LVGL/src/core/lv_indev_scroll.c \
../my_drivers/LVGL/src/core/lv_obj.c \
../my_drivers/LVGL/src/core/lv_obj_class.c \
../my_drivers/LVGL/src/core/lv_obj_draw.c \
../my_drivers/LVGL/src/core/lv_obj_pos.c \
../my_drivers/LVGL/src/core/lv_obj_scroll.c \
../my_drivers/LVGL/src/core/lv_obj_style.c \
../my_drivers/LVGL/src/core/lv_obj_style_gen.c \
../my_drivers/LVGL/src/core/lv_obj_tree.c \
../my_drivers/LVGL/src/core/lv_refr.c \
../my_drivers/LVGL/src/core/lv_theme.c 

OBJS += \
./my_drivers/LVGL/src/core/lv_disp.o \
./my_drivers/LVGL/src/core/lv_event.o \
./my_drivers/LVGL/src/core/lv_group.o \
./my_drivers/LVGL/src/core/lv_indev.o \
./my_drivers/LVGL/src/core/lv_indev_scroll.o \
./my_drivers/LVGL/src/core/lv_obj.o \
./my_drivers/LVGL/src/core/lv_obj_class.o \
./my_drivers/LVGL/src/core/lv_obj_draw.o \
./my_drivers/LVGL/src/core/lv_obj_pos.o \
./my_drivers/LVGL/src/core/lv_obj_scroll.o \
./my_drivers/LVGL/src/core/lv_obj_style.o \
./my_drivers/LVGL/src/core/lv_obj_style_gen.o \
./my_drivers/LVGL/src/core/lv_obj_tree.o \
./my_drivers/LVGL/src/core/lv_refr.o \
./my_drivers/LVGL/src/core/lv_theme.o 

C_DEPS += \
./my_drivers/LVGL/src/core/lv_disp.d \
./my_drivers/LVGL/src/core/lv_event.d \
./my_drivers/LVGL/src/core/lv_group.d \
./my_drivers/LVGL/src/core/lv_indev.d \
./my_drivers/LVGL/src/core/lv_indev_scroll.d \
./my_drivers/LVGL/src/core/lv_obj.d \
./my_drivers/LVGL/src/core/lv_obj_class.d \
./my_drivers/LVGL/src/core/lv_obj_draw.d \
./my_drivers/LVGL/src/core/lv_obj_pos.d \
./my_drivers/LVGL/src/core/lv_obj_scroll.d \
./my_drivers/LVGL/src/core/lv_obj_style.d \
./my_drivers/LVGL/src/core/lv_obj_style_gen.d \
./my_drivers/LVGL/src/core/lv_obj_tree.d \
./my_drivers/LVGL/src/core/lv_refr.d \
./my_drivers/LVGL/src/core/lv_theme.d 


# 每个子目录必须为构建它所贡献的源提供规则
my_drivers/LVGL/src/core/%.o my_drivers/LVGL/src/core/%.su my_drivers/LVGL/src/core/%.cyclo: ../my_drivers/LVGL/src/core/%.c my_drivers/LVGL/src/core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../SCSLib -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/src -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/examples/porting -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/User/Inc -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/demos/music -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/demos/benchmark -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/FreeRtos/portable -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/FreeRtos/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-my_drivers-2f-LVGL-2f-src-2f-core

clean-my_drivers-2f-LVGL-2f-src-2f-core:
	-$(RM) ./my_drivers/LVGL/src/core/lv_disp.cyclo ./my_drivers/LVGL/src/core/lv_disp.d ./my_drivers/LVGL/src/core/lv_disp.o ./my_drivers/LVGL/src/core/lv_disp.su ./my_drivers/LVGL/src/core/lv_event.cyclo ./my_drivers/LVGL/src/core/lv_event.d ./my_drivers/LVGL/src/core/lv_event.o ./my_drivers/LVGL/src/core/lv_event.su ./my_drivers/LVGL/src/core/lv_group.cyclo ./my_drivers/LVGL/src/core/lv_group.d ./my_drivers/LVGL/src/core/lv_group.o ./my_drivers/LVGL/src/core/lv_group.su ./my_drivers/LVGL/src/core/lv_indev.cyclo ./my_drivers/LVGL/src/core/lv_indev.d ./my_drivers/LVGL/src/core/lv_indev.o ./my_drivers/LVGL/src/core/lv_indev.su ./my_drivers/LVGL/src/core/lv_indev_scroll.cyclo ./my_drivers/LVGL/src/core/lv_indev_scroll.d ./my_drivers/LVGL/src/core/lv_indev_scroll.o ./my_drivers/LVGL/src/core/lv_indev_scroll.su ./my_drivers/LVGL/src/core/lv_obj.cyclo ./my_drivers/LVGL/src/core/lv_obj.d ./my_drivers/LVGL/src/core/lv_obj.o ./my_drivers/LVGL/src/core/lv_obj.su ./my_drivers/LVGL/src/core/lv_obj_class.cyclo ./my_drivers/LVGL/src/core/lv_obj_class.d ./my_drivers/LVGL/src/core/lv_obj_class.o ./my_drivers/LVGL/src/core/lv_obj_class.su ./my_drivers/LVGL/src/core/lv_obj_draw.cyclo ./my_drivers/LVGL/src/core/lv_obj_draw.d ./my_drivers/LVGL/src/core/lv_obj_draw.o ./my_drivers/LVGL/src/core/lv_obj_draw.su ./my_drivers/LVGL/src/core/lv_obj_pos.cyclo ./my_drivers/LVGL/src/core/lv_obj_pos.d ./my_drivers/LVGL/src/core/lv_obj_pos.o ./my_drivers/LVGL/src/core/lv_obj_pos.su ./my_drivers/LVGL/src/core/lv_obj_scroll.cyclo ./my_drivers/LVGL/src/core/lv_obj_scroll.d ./my_drivers/LVGL/src/core/lv_obj_scroll.o ./my_drivers/LVGL/src/core/lv_obj_scroll.su ./my_drivers/LVGL/src/core/lv_obj_style.cyclo ./my_drivers/LVGL/src/core/lv_obj_style.d ./my_drivers/LVGL/src/core/lv_obj_style.o ./my_drivers/LVGL/src/core/lv_obj_style.su ./my_drivers/LVGL/src/core/lv_obj_style_gen.cyclo ./my_drivers/LVGL/src/core/lv_obj_style_gen.d ./my_drivers/LVGL/src/core/lv_obj_style_gen.o ./my_drivers/LVGL/src/core/lv_obj_style_gen.su ./my_drivers/LVGL/src/core/lv_obj_tree.cyclo ./my_drivers/LVGL/src/core/lv_obj_tree.d ./my_drivers/LVGL/src/core/lv_obj_tree.o ./my_drivers/LVGL/src/core/lv_obj_tree.su ./my_drivers/LVGL/src/core/lv_refr.cyclo ./my_drivers/LVGL/src/core/lv_refr.d ./my_drivers/LVGL/src/core/lv_refr.o ./my_drivers/LVGL/src/core/lv_refr.su ./my_drivers/LVGL/src/core/lv_theme.cyclo ./my_drivers/LVGL/src/core/lv_theme.d ./my_drivers/LVGL/src/core/lv_theme.o ./my_drivers/LVGL/src/core/lv_theme.su

.PHONY: clean-my_drivers-2f-LVGL-2f-src-2f-core

