################################################################################
# 自动生成的文件。不要编辑！
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
../my_drivers/LVGL/demos/widgets/assets/img_clothes.c \
../my_drivers/LVGL/demos/widgets/assets/img_demo_widgets_avatar.c \
../my_drivers/LVGL/demos/widgets/assets/img_lvgl_logo.c 

OBJS += \
./my_drivers/LVGL/demos/widgets/assets/img_clothes.o \
./my_drivers/LVGL/demos/widgets/assets/img_demo_widgets_avatar.o \
./my_drivers/LVGL/demos/widgets/assets/img_lvgl_logo.o 

C_DEPS += \
./my_drivers/LVGL/demos/widgets/assets/img_clothes.d \
./my_drivers/LVGL/demos/widgets/assets/img_demo_widgets_avatar.d \
./my_drivers/LVGL/demos/widgets/assets/img_lvgl_logo.d 


# 每个子目录必须为构建它所贡献的源提供规则
my_drivers/LVGL/demos/widgets/assets/%.o my_drivers/LVGL/demos/widgets/assets/%.su my_drivers/LVGL/demos/widgets/assets/%.cyclo: ../my_drivers/LVGL/demos/widgets/assets/%.c my_drivers/LVGL/demos/widgets/assets/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../SCSLib -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/src -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/examples/porting -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/User/Inc -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/demos/music -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/demos/benchmark -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/FreeRtos/portable -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/FreeRtos/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-my_drivers-2f-LVGL-2f-demos-2f-widgets-2f-assets

clean-my_drivers-2f-LVGL-2f-demos-2f-widgets-2f-assets:
	-$(RM) ./my_drivers/LVGL/demos/widgets/assets/img_clothes.cyclo ./my_drivers/LVGL/demos/widgets/assets/img_clothes.d ./my_drivers/LVGL/demos/widgets/assets/img_clothes.o ./my_drivers/LVGL/demos/widgets/assets/img_clothes.su ./my_drivers/LVGL/demos/widgets/assets/img_demo_widgets_avatar.cyclo ./my_drivers/LVGL/demos/widgets/assets/img_demo_widgets_avatar.d ./my_drivers/LVGL/demos/widgets/assets/img_demo_widgets_avatar.o ./my_drivers/LVGL/demos/widgets/assets/img_demo_widgets_avatar.su ./my_drivers/LVGL/demos/widgets/assets/img_lvgl_logo.cyclo ./my_drivers/LVGL/demos/widgets/assets/img_lvgl_logo.d ./my_drivers/LVGL/demos/widgets/assets/img_lvgl_logo.o ./my_drivers/LVGL/demos/widgets/assets/img_lvgl_logo.su

.PHONY: clean-my_drivers-2f-LVGL-2f-demos-2f-widgets-2f-assets

