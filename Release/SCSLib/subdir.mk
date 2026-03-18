################################################################################
# 自动生成的文件。不要编辑！
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
../SCSLib/HLS.c \
../SCSLib/SCS.c \
../SCSLib/SCSCL.c \
../SCSLib/SCSerail.c \
../SCSLib/SMS_STS.c 

OBJS += \
./SCSLib/HLS.o \
./SCSLib/SCS.o \
./SCSLib/SCSCL.o \
./SCSLib/SCSerail.o \
./SCSLib/SMS_STS.o 

C_DEPS += \
./SCSLib/HLS.d \
./SCSLib/SCS.d \
./SCSLib/SCSCL.d \
./SCSLib/SCSerail.d \
./SCSLib/SMS_STS.d 


# 每个子目录必须为构建它所贡献的源提供规则
SCSLib/%.o SCSLib/%.su SCSLib/%.cyclo: ../SCSLib/%.c SCSLib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../SCSLib -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/src -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/examples/porting -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/User/Inc -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/demos/music -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/my_drivers/LVGL/demos/benchmark -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/FreeRtos/portable -I/home/ouo/li_lvgl/lv_motor/feetech_servo_h750/FreeRtos/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-SCSLib

clean-SCSLib:
	-$(RM) ./SCSLib/HLS.cyclo ./SCSLib/HLS.d ./SCSLib/HLS.o ./SCSLib/HLS.su ./SCSLib/SCS.cyclo ./SCSLib/SCS.d ./SCSLib/SCS.o ./SCSLib/SCS.su ./SCSLib/SCSCL.cyclo ./SCSLib/SCSCL.d ./SCSLib/SCSCL.o ./SCSLib/SCSCL.su ./SCSLib/SCSerail.cyclo ./SCSLib/SCSerail.d ./SCSLib/SCSerail.o ./SCSLib/SCSerail.su ./SCSLib/SMS_STS.cyclo ./SCSLib/SMS_STS.d ./SCSLib/SMS_STS.o ./SCSLib/SMS_STS.su

.PHONY: clean-SCSLib

