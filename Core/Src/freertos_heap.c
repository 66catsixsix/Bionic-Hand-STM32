/**
 * @file  freertos_heap.c
 * @brief 把 FreeRTOS 堆放在外部 SDRAM (0xC0000000)
 *
 * 条件：FreeRTOSConfig.h 中必须设置
 *   #define configAPPLICATION_ALLOCATED_HEAP  1
 *   #define configTOTAL_HEAP_SIZE             ( 2 * 1024 * 1024 )
 *
 * 链接脚本中 .sdram_bss 段 (NOLOAD) 已映射到 SDRAM 区域。
 * heap_4.c 在第一次 pvPortMalloc() 时会自行初始化该区域，
 * 无需 startup 代码预先清零。
 */

#include "FreeRTOS.h"

/* configTOTAL_HEAP_SIZE 来自 FreeRTOSConfig.h */
uint8_t ucHeap[ configTOTAL_HEAP_SIZE ]
    __attribute__((section(".sdram_bss")))
    __attribute__((aligned(8)));
