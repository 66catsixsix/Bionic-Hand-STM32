/*
** STM32H750XBHX_FLASH.ld
** 反客FK750M5-XBH6核心板 — 代码全部运行于外部QSPI Flash (XIP方式)
** QSPI Flash 内存映射地址: 0x90000000，大小8MB
** 与屏幕工程保持一致，VTOR需在system_stm32h7xx.c中设为0x90000000
*/

ENTRY(Reset_Handler)

_estack = ORIGIN(RAM_D1) + LENGTH(RAM_D1);
_Min_Heap_Size  = 0x200;
_Min_Stack_Size = 0x400;

MEMORY
{
  FLASH    (rx)  : ORIGIN = 0x08000000, LENGTH = 128K   /* 内部Flash，仅存Boot */
  QSPI     (rx)  : ORIGIN = 0x90000000, LENGTH = 8M     /* 外部QSPI Flash，存应用代码 */
  DTCMRAM  (xrw) : ORIGIN = 0x20000000, LENGTH = 128K
  RAM_D1   (xrw) : ORIGIN = 0x24000000, LENGTH = 512K
  RAM_D2   (xrw) : ORIGIN = 0x30000000, LENGTH = 288K
  RAM_D3   (xrw) : ORIGIN = 0x38000000, LENGTH = 64K
  ITCMRAM  (xrw) : ORIGIN = 0x00000000, LENGTH = 64K
  SDRAM    (xrw) : ORIGIN = 0xC0000000, LENGTH = 32M    /* 板载 32MB SDRAM (FMC) */
}

SECTIONS
{
  /* 向量表放在QSPI最前面 */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector))
    . = ALIGN(4);
  } >QSPI

  /* 代码段 */
  .text :
  {
    . = ALIGN(4);
    *(.text)
    *(.text*)
    *(.glue_7)
    *(.glue_7t)
    *(.eh_frame)
    *(.RamFunc)
    *(.RamFunc*)
    KEEP (*(.init))
    KEEP (*(.fini))
    . = ALIGN(4);
    _etext = .;
  } >QSPI

  /* 只读数据 */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)
    *(.rodata*)
    . = ALIGN(4);
  } >QSPI

  .ARM.extab : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >QSPI
  .ARM :
  {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } >QSPI

  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >QSPI

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >QSPI

  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >QSPI

  /* 初始化数据：存在QSPI，运行时复制到RAM_D1 */
  _sidata = LOADADDR(.data);
  .data :
  {
    . = ALIGN(4);
    _sdata = .;
    *(.data)
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } >RAM_D1 AT> QSPI

  /* 未初始化数据 */
  . = ALIGN(4);
  .bss :
  {
    _sbss = .;
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
    __bss_end__ = _ebss;
  } >RAM_D1

  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >RAM_D1

  /* FreeRTOS 堆：放在外部 SDRAM，不占 Flash，启动后由 heap_4.c 自行初始化 */
  .sdram_bss (NOLOAD) :
  {
    . = ALIGN(8);
    _sdram_bss_start = .;
    *(.sdram_bss)
    *(.sdram_bss*)
    . = ALIGN(8);
    _sdram_bss_end = .;
  } >SDRAM

  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}
