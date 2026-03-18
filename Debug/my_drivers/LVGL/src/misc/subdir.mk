################################################################################
# 自动生成的文件。不要编辑！
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
../my_drivers/LVGL/src/misc/lv_anim.c \
../my_drivers/LVGL/src/misc/lv_anim_timeline.c \
../my_drivers/LVGL/src/misc/lv_area.c \
../my_drivers/LVGL/src/misc/lv_async.c \
../my_drivers/LVGL/src/misc/lv_bidi.c \
../my_drivers/LVGL/src/misc/lv_color.c \
../my_drivers/LVGL/src/misc/lv_fs.c \
../my_drivers/LVGL/src/misc/lv_gc.c \
../my_drivers/LVGL/src/misc/lv_ll.c \
../my_drivers/LVGL/src/misc/lv_log.c \
../my_drivers/LVGL/src/misc/lv_lru.c \
../my_drivers/LVGL/src/misc/lv_math.c \
../my_drivers/LVGL/src/misc/lv_mem.c \
../my_drivers/LVGL/src/misc/lv_printf.c \
../my_drivers/LVGL/src/misc/lv_style.c \
../my_drivers/LVGL/src/misc/lv_style_gen.c \
../my_drivers/LVGL/src/misc/lv_templ.c \
../my_drivers/LVGL/src/misc/lv_timer.c \
../my_drivers/LVGL/src/misc/lv_tlsf.c \
../my_drivers/LVGL/src/misc/lv_txt.c \
../my_drivers/LVGL/src/misc/lv_txt_ap.c \
../my_drivers/LVGL/src/misc/lv_utils.c 

OBJS += \
./my_drivers/LVGL/src/misc/lv_anim.o \
./my_drivers/LVGL/src/misc/lv_anim_timeline.o \
./my_drivers/LVGL/src/misc/lv_area.o \
./my_drivers/LVGL/src/misc/lv_async.o \
./my_drivers/LVGL/src/misc/lv_bidi.o \
./my_drivers/LVGL/src/misc/lv_color.o \
./my_drivers/LVGL/src/misc/lv_fs.o \
./my_drivers/LVGL/src/misc/lv_gc.o \
./my_drivers/LVGL/src/misc/lv_ll.o \
./my_drivers/LVGL/src/misc/lv_log.o \
./my_drivers/LVGL/src/misc/lv_lru.o \
./my_drivers/LVGL/src/misc/lv_math.o \
./my_drivers/LVGL/src/misc/lv_mem.o \
./my_drivers/LVGL/src/misc/lv_printf.o \
./my_drivers/LVGL/src/misc/lv_style.o \
./my_drivers/LVGL/src/misc/lv_style_gen.o \
./my_drivers/LVGL/src/misc/lv_templ.o \
./my_drivers/LVGL/src/misc/lv_timer.o \
./my_drivers/LVGL/src/misc/lv_tlsf.o \
./my_drivers/LVGL/src/misc/lv_txt.o \
./my_drivers/LVGL/src/misc/lv_txt_ap.o \
./my_drivers/LVGL/src/misc/lv_utils.o 

C_DEPS += \
./my_drivers/LVGL/src/misc/lv_anim.d \
./my_drivers/LVGL/src/misc/lv_anim_timeline.d \
./my_drivers/LVGL/src/misc/lv_area.d \
./my_drivers/LVGL/src/misc/lv_async.d \
./my_drivers/LVGL/src/misc/lv_bidi.d \
./my_drivers/LVGL/src/misc/lv_color.d \
./my_drivers/LVGL/src/misc/lv_fs.d \
./my_drivers/LVGL/src/misc/lv_gc.d \
./my_drivers/LVGL/src/misc/lv_ll.d \
./my_drivers/LVGL/src/misc/lv_log.d \
./my_drivers/LVGL/src/misc/lv_lru.d \
./my_drivers/LVGL/src/misc/lv_math.d \
./my_drivers/LVGL/src/misc/lv_mem.d \
./my_drivers/LVGL/src/misc/lv_printf.d \
./my_drivers/LVGL/src/misc/lv_style.d \
./my_drivers/LVGL/src/misc/lv_style_gen.d \
./my_drivers/LVGL/src/misc/lv_templ.d \
./my_drivers/LVGL/src/misc/lv_timer.d \
./my_drivers/LVGL/src/misc/lv_tlsf.d \
./my_drivers/LVGL/src/misc/lv_txt.d \
./my_drivers/LVGL/src/misc/lv_txt_ap.d \
./my_drivers/LVGL/src/misc/lv_utils.d 


# 每个子目录必须为构建它所贡献的源提供规则
my_drivers/LVGL/src/misc/%.o my_drivers/LVGL/src/misc/%.su my_drivers/LVGL/src/misc/%.cyclo: ../my_drivers/LVGL/src/misc/%.c my_drivers/LVGL/src/misc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../SCSLib -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/src -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/examples/porting -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/User/Inc -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/demos/music -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/demos/benchmark -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/FreeRtos/portable -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/FreeRtos/include -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_core/ui_H -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_core/feetech_motor/inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-my_drivers-2f-LVGL-2f-src-2f-misc

clean-my_drivers-2f-LVGL-2f-src-2f-misc:
	-$(RM) ./my_drivers/LVGL/src/misc/lv_anim.cyclo ./my_drivers/LVGL/src/misc/lv_anim.d ./my_drivers/LVGL/src/misc/lv_anim.o ./my_drivers/LVGL/src/misc/lv_anim.su ./my_drivers/LVGL/src/misc/lv_anim_timeline.cyclo ./my_drivers/LVGL/src/misc/lv_anim_timeline.d ./my_drivers/LVGL/src/misc/lv_anim_timeline.o ./my_drivers/LVGL/src/misc/lv_anim_timeline.su ./my_drivers/LVGL/src/misc/lv_area.cyclo ./my_drivers/LVGL/src/misc/lv_area.d ./my_drivers/LVGL/src/misc/lv_area.o ./my_drivers/LVGL/src/misc/lv_area.su ./my_drivers/LVGL/src/misc/lv_async.cyclo ./my_drivers/LVGL/src/misc/lv_async.d ./my_drivers/LVGL/src/misc/lv_async.o ./my_drivers/LVGL/src/misc/lv_async.su ./my_drivers/LVGL/src/misc/lv_bidi.cyclo ./my_drivers/LVGL/src/misc/lv_bidi.d ./my_drivers/LVGL/src/misc/lv_bidi.o ./my_drivers/LVGL/src/misc/lv_bidi.su ./my_drivers/LVGL/src/misc/lv_color.cyclo ./my_drivers/LVGL/src/misc/lv_color.d ./my_drivers/LVGL/src/misc/lv_color.o ./my_drivers/LVGL/src/misc/lv_color.su ./my_drivers/LVGL/src/misc/lv_fs.cyclo ./my_drivers/LVGL/src/misc/lv_fs.d ./my_drivers/LVGL/src/misc/lv_fs.o ./my_drivers/LVGL/src/misc/lv_fs.su ./my_drivers/LVGL/src/misc/lv_gc.cyclo ./my_drivers/LVGL/src/misc/lv_gc.d ./my_drivers/LVGL/src/misc/lv_gc.o ./my_drivers/LVGL/src/misc/lv_gc.su ./my_drivers/LVGL/src/misc/lv_ll.cyclo ./my_drivers/LVGL/src/misc/lv_ll.d ./my_drivers/LVGL/src/misc/lv_ll.o ./my_drivers/LVGL/src/misc/lv_ll.su ./my_drivers/LVGL/src/misc/lv_log.cyclo ./my_drivers/LVGL/src/misc/lv_log.d ./my_drivers/LVGL/src/misc/lv_log.o ./my_drivers/LVGL/src/misc/lv_log.su ./my_drivers/LVGL/src/misc/lv_lru.cyclo ./my_drivers/LVGL/src/misc/lv_lru.d ./my_drivers/LVGL/src/misc/lv_lru.o ./my_drivers/LVGL/src/misc/lv_lru.su ./my_drivers/LVGL/src/misc/lv_math.cyclo ./my_drivers/LVGL/src/misc/lv_math.d ./my_drivers/LVGL/src/misc/lv_math.o ./my_drivers/LVGL/src/misc/lv_math.su ./my_drivers/LVGL/src/misc/lv_mem.cyclo ./my_drivers/LVGL/src/misc/lv_mem.d ./my_drivers/LVGL/src/misc/lv_mem.o ./my_drivers/LVGL/src/misc/lv_mem.su ./my_drivers/LVGL/src/misc/lv_printf.cyclo ./my_drivers/LVGL/src/misc/lv_printf.d ./my_drivers/LVGL/src/misc/lv_printf.o ./my_drivers/LVGL/src/misc/lv_printf.su ./my_drivers/LVGL/src/misc/lv_style.cyclo ./my_drivers/LVGL/src/misc/lv_style.d ./my_drivers/LVGL/src/misc/lv_style.o ./my_drivers/LVGL/src/misc/lv_style.su ./my_drivers/LVGL/src/misc/lv_style_gen.cyclo ./my_drivers/LVGL/src/misc/lv_style_gen.d ./my_drivers/LVGL/src/misc/lv_style_gen.o ./my_drivers/LVGL/src/misc/lv_style_gen.su ./my_drivers/LVGL/src/misc/lv_templ.cyclo ./my_drivers/LVGL/src/misc/lv_templ.d ./my_drivers/LVGL/src/misc/lv_templ.o ./my_drivers/LVGL/src/misc/lv_templ.su ./my_drivers/LVGL/src/misc/lv_timer.cyclo ./my_drivers/LVGL/src/misc/lv_timer.d ./my_drivers/LVGL/src/misc/lv_timer.o ./my_drivers/LVGL/src/misc/lv_timer.su ./my_drivers/LVGL/src/misc/lv_tlsf.cyclo ./my_drivers/LVGL/src/misc/lv_tlsf.d ./my_drivers/LVGL/src/misc/lv_tlsf.o ./my_drivers/LVGL/src/misc/lv_tlsf.su ./my_drivers/LVGL/src/misc/lv_txt.cyclo ./my_drivers/LVGL/src/misc/lv_txt.d ./my_drivers/LVGL/src/misc/lv_txt.o ./my_drivers/LVGL/src/misc/lv_txt.su ./my_drivers/LVGL/src/misc/lv_txt_ap.cyclo ./my_drivers/LVGL/src/misc/lv_txt_ap.d ./my_drivers/LVGL/src/misc/lv_txt_ap.o ./my_drivers/LVGL/src/misc/lv_txt_ap.su ./my_drivers/LVGL/src/misc/lv_utils.cyclo ./my_drivers/LVGL/src/misc/lv_utils.d ./my_drivers/LVGL/src/misc/lv_utils.o ./my_drivers/LVGL/src/misc/lv_utils.su

.PHONY: clean-my_drivers-2f-LVGL-2f-src-2f-misc

