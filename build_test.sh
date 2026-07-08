#!/bin/bash
# 验证编译脚本 — 仅用于测试，不是CubeIDE的构建脚本
PROJ=/home/ouo/li_lvgl/lv_motor/feetech_servo_h750
GCC=/opt/st/stm32cubeide_2.0.0/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.13.3.rel1.linux64_1.0.100.202509120712/tools/bin/arm-none-eabi-gcc
SIZE=/opt/st/stm32cubeide_2.0.0/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.13.3.rel1.linux64_1.0.100.202509120712/tools/bin/arm-none-eabi-size
OBJCOPY=/opt/st/stm32cubeide_2.0.0/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.13.3.rel1.linux64_1.0.100.202509120712/tools/bin/arm-none-eabi-objcopy

OUT=/tmp/h750_build
mkdir -p $OUT

CFLAGS="-mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard -DUSE_HAL_DRIVER -DSTM32H750xx -DDEBUG -std=gnu11 -w -g3"
INCS="-I$PROJ/Core/Inc \
      -I$PROJ/Drivers/STM32H7xx_HAL_Driver/Inc \
      -I$PROJ/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy \
      -I$PROJ/Drivers/CMSIS/Device/ST/STM32H7xx/Include \
      -I$PROJ/Drivers/CMSIS/Include \
      -I$PROJ/SCSLib \
      -I$PROJ/my_drivers \
      -I$PROJ/my_drivers/LVGL \
      -I$PROJ/my_drivers/LVGL/examples/porting \
      -I$PROJ/my_drivers/User/Inc \
      -I$PROJ/my_core/feetech_motor/inc \
      -I$PROJ/my_core/ui_H"

OBJS=()

compile_file() {
    src=$1
    out=$2
    if $GCC $CFLAGS $INCS -c "$src" -o "$out"; then
        echo "  [OK] $(basename $src)"
        OBJS+=("$out")
    else
        echo "  [FAIL] $(basename $src)"
        exit 1
    fi
}

echo "=== 编译 HAL 驱动 ==="
for f in stm32h7xx_hal.c stm32h7xx_hal_rcc.c stm32h7xx_hal_rcc_ex.c \
          stm32h7xx_hal_gpio.c stm32h7xx_hal_uart.c stm32h7xx_hal_uart_ex.c \
          stm32h7xx_hal_usart.c stm32h7xx_hal_usart_ex.c \
          stm32h7xx_hal_cortex.c stm32h7xx_hal_pwr.c stm32h7xx_hal_pwr_ex.c \
          stm32h7xx_hal_flash.c stm32h7xx_hal_flash_ex.c \
          stm32h7xx_hal_dma.c stm32h7xx_hal_dma_ex.c stm32h7xx_hal_mdma.c \
          stm32h7xx_hal_tim.c stm32h7xx_hal_tim_ex.c \
          stm32h7xx_hal_dma2d.c \
          stm32h7xx_hal_ltdc.c stm32h7xx_hal_ltdc_ex.c \
          stm32h7xx_hal_sdram.c stm32h7xx_ll_fmc.c; do
    compile_file "$PROJ/Drivers/STM32H7xx_HAL_Driver/Src/$f" "$OUT/hal_${f%.c}.o"
done

echo ""
echo "=== 编译 Core 源文件 ==="
for src in "$PROJ/Core/Src/main.c" "$PROJ/Core/Src/usart.c" "$PROJ/Core/Src/gpio.c" \
           "$PROJ/Core/Src/stm32h7xx_hal_msp.c" "$PROJ/Core/Src/stm32h7xx_it.c" \
           "$PROJ/Core/Src/system_stm32h7xx.c" "$PROJ/Core/Src/syscalls.c" "$PROJ/Core/Src/sysmem.c"; do
    base=$(basename "$src")
    compile_file "$src" "$OUT/${base%.c}.o"
done

echo ""
echo "=== 编译 SCSLib ==="
for src in "$PROJ/SCSLib/SCSerail.c" "$PROJ/SCSLib/SCS.c" "$PROJ/SCSLib/SCSCL.c" \
           "$PROJ/SCSLib/SMS_STS.c" "$PROJ/SCSLib/HLS.c"; do
    base=$(basename "$src")
    compile_file "$src" "$OUT/${base%.c}.o"
done

echo ""
echo "=== 编译 my_drivers/User/Src ==="
for src in "$PROJ/my_drivers/User/Src/lcd_rgb.c" \
           "$PROJ/my_drivers/User/Src/sdram.c" \
           "$PROJ/my_drivers/User/Src/touch_800x480.c" \
           "$PROJ/my_drivers/User/Src/touch_iic.c"; do
    base=$(basename "$src")
    compile_file "$src" "$OUT/user_${base%.c}.o"
done

echo ""
echo "=== 编译 LVGL ==="
# 排除非 STM32 平台后端（SDL/NXP/ARM2D/SWM341）
while IFS= read -r src; do
    rel="${src#$PROJ/}"
    # 用相对路径生成不重名的目标文件名
    out_name=$(echo "$rel" | tr '/' '_')
    compile_file "$src" "$OUT/${out_name%.c}.o"
done < <(find "$PROJ/my_drivers/LVGL" -name "*.c" \
    | grep -v "/demos/" \
    | grep -v "/draw/sdl/" \
    | grep -v "/draw/nxp/" \
    | grep -v "/draw/arm2d/" \
    | grep -v "/draw/swm341_dma2d/" \
    | grep -v "/extra/libs/ffmpeg/" \
    | grep -v "/extra/libs/freetype/" \
    | grep -v "/extra/libs/rlottie/" \
    | sort)

echo ""
echo "=== 编译 my_core ==="
for src in "$PROJ/my_core/feetech_motor/src/feetech_motor.c" \
           "$PROJ/my_core/ui/main_screen.c" \
           "$PROJ/my_core/ui/lvgl_motor.c"; do
    base=$(basename "$src")
    compile_file "$src" "$OUT/mycore_${base%.c}.o"
done

echo ""
echo "=== 编译启动文件 ==="
$GCC $CFLAGS -x assembler-with-cpp -c "$PROJ/Core/Startup/startup_stm32h750xbhx.s" -o "$OUT/startup.o"
OBJS+=("$OUT/startup.o")
echo "  [OK] startup_stm32h750xbhx.s"

echo ""
echo "=== 链接 ==="
$GCC -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard \
    -T"$PROJ/STM32H750XBHX_FLASH.ld" \
    --specs=nano.specs -lc -lm \
    -Wl,-Map="$OUT/feetech_servo_h750.map",--cref -Wl,--gc-sections \
    "${OBJS[@]}" \
    -o "$OUT/feetech_servo_h750.elf"

echo ""
echo "=== 大小报告 ==="
$SIZE "$OUT/feetech_servo_h750.elf"

$OBJCOPY -O ihex "$OUT/feetech_servo_h750.elf" "$OUT/feetech_servo_h750.hex"
$OBJCOPY -O binary "$OUT/feetech_servo_h750.elf" "$OUT/feetech_servo_h750.bin"

echo ""
echo "✅ 构建完成!"
echo "   ELF: $OUT/feetech_servo_h750.elf"
echo "   HEX: $OUT/feetech_servo_h750.hex"
echo "   BIN: $OUT/feetech_servo_h750.bin"
