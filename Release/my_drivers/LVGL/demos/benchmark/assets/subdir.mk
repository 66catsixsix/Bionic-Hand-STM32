################################################################################
# 自动生成的文件。不要编辑！
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
../my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_alpha16.c \
../my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_argb.c \
../my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_chroma_keyed.c \
../my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_indexed16.c \
../my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_rgb.c \
../my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_rgb565a8.c \
../my_drivers/LVGL/demos/benchmark/assets/lv_font_bechmark_montserrat_12_compr_az.c.c \
../my_drivers/LVGL/demos/benchmark/assets/lv_font_bechmark_montserrat_16_compr_az.c.c \
../my_drivers/LVGL/demos/benchmark/assets/lv_font_bechmark_montserrat_28_compr_az.c.c 

OBJS += \
./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_alpha16.o \
./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_argb.o \
./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_chroma_keyed.o \
./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_indexed16.o \
./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_rgb.o \
./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_rgb565a8.o \
./my_drivers/LVGL/demos/benchmark/assets/lv_font_bechmark_montserrat_12_compr_az.c.o \
./my_drivers/LVGL/demos/benchmark/assets/lv_font_bechmark_montserrat_16_compr_az.c.o \
./my_drivers/LVGL/demos/benchmark/assets/lv_font_bechmark_montserrat_28_compr_az.c.o 

C_DEPS += \
./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_alpha16.d \
./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_argb.d \
./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_chroma_keyed.d \
./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_indexed16.d \
./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_rgb.d \
./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_rgb565a8.d \
./my_drivers/LVGL/demos/benchmark/assets/lv_font_bechmark_montserrat_12_compr_az.c.d \
./my_drivers/LVGL/demos/benchmark/assets/lv_font_bechmark_montserrat_16_compr_az.c.d \
./my_drivers/LVGL/demos/benchmark/assets/lv_font_bechmark_montserrat_28_compr_az.c.d 


# 每个子目录必须为构建它所贡献的源提供规则
my_drivers/LVGL/demos/benchmark/assets/%.o my_drivers/LVGL/demos/benchmark/assets/%.su my_drivers/LVGL/demos/benchmark/assets/%.cyclo: ../my_drivers/LVGL/demos/benchmark/assets/%.c my_drivers/LVGL/demos/benchmark/assets/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../SCSLib -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/src -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/examples/porting -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/User/Inc -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/demos/music -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/demos/benchmark -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/FreeRtos/portable -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/FreeRtos/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-my_drivers-2f-LVGL-2f-demos-2f-benchmark-2f-assets

clean-my_drivers-2f-LVGL-2f-demos-2f-benchmark-2f-assets:
	-$(RM) ./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_alpha16.cyclo ./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_alpha16.d ./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_alpha16.o ./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_alpha16.su ./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_argb.cyclo ./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_argb.d ./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_argb.o ./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_argb.su ./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_chroma_keyed.cyclo ./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_chroma_keyed.d ./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_chroma_keyed.o ./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_chroma_keyed.su ./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_indexed16.cyclo ./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_indexed16.d ./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_indexed16.o ./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_indexed16.su ./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_rgb.cyclo ./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_rgb.d ./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_rgb.o ./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_rgb.su ./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_rgb565a8.cyclo ./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_rgb565a8.d ./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_rgb565a8.o ./my_drivers/LVGL/demos/benchmark/assets/img_benchmark_cogwheel_rgb565a8.su ./my_drivers/LVGL/demos/benchmark/assets/lv_font_bechmark_montserrat_12_compr_az.c.cyclo ./my_drivers/LVGL/demos/benchmark/assets/lv_font_bechmark_montserrat_12_compr_az.c.d ./my_drivers/LVGL/demos/benchmark/assets/lv_font_bechmark_montserrat_12_compr_az.c.o ./my_drivers/LVGL/demos/benchmark/assets/lv_font_bechmark_montserrat_12_compr_az.c.su ./my_drivers/LVGL/demos/benchmark/assets/lv_font_bechmark_montserrat_16_compr_az.c.cyclo ./my_drivers/LVGL/demos/benchmark/assets/lv_font_bechmark_montserrat_16_compr_az.c.d ./my_drivers/LVGL/demos/benchmark/assets/lv_font_bechmark_montserrat_16_compr_az.c.o ./my_drivers/LVGL/demos/benchmark/assets/lv_font_bechmark_montserrat_16_compr_az.c.su ./my_drivers/LVGL/demos/benchmark/assets/lv_font_bechmark_montserrat_28_compr_az.c.cyclo ./my_drivers/LVGL/demos/benchmark/assets/lv_font_bechmark_montserrat_28_compr_az.c.d ./my_drivers/LVGL/demos/benchmark/assets/lv_font_bechmark_montserrat_28_compr_az.c.o ./my_drivers/LVGL/demos/benchmark/assets/lv_font_bechmark_montserrat_28_compr_az.c.su

.PHONY: clean-my_drivers-2f-LVGL-2f-demos-2f-benchmark-2f-assets

