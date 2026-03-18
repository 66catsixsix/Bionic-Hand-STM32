/**
  ******************************************************************************
  * @file    main.c
  * @brief   飞特 SCS2332 舵机驱动 — STM32H750XBH6 (反客FK750M5-XBH6)
  *          UART: USART1 PA9 半双工单总线 1Mbps
  *          时钟: HSE 25MHz → PLL → 480MHz SYSCLK
  ******************************************************************************
  * 接线:
  *   PA9  ────  舵机信号线 (S), 开漏上拉, 单根线半双工
  *   GND  ────  舵机 GND
  *   舵机VCC ── 6~8.4V 外部供电 (勿接H750的3.3V!)
  ******************************************************************************
  */

#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "SCServo.h"

/* LVGL + 屏幕驱动 */
#include "sdram.h"
#include "lcd_rgb.h"
#include "touch_800x480.h"
#include "lvgl.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lv_conf.h"
#include "lv_demo_benchmark.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lvgl_motor.h"
#include "feetech_motor.h"
/* 触摸屏是否就绪标志 */
static uint8_t touch_ok = 0;

//GlobalVar
volatile int16_t servo_pos_now = 0;


/* ======================== FreeRTOS 任务函数 ======================== */

static void task_lvgl(void *arg)
{
    (void)arg;
    //lv_demo_widgets();
    lvgl_motor();
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(10); // 与 LV_DISP_DEF_REFR_PERIOD 一致
    
    for (;;) {
        lv_task_handler();
        if (touch_ok) Touch_Scan();
        vTaskDelayUntil(&xLastWakeTime, xPeriod); // 严格周期，不受渲染耗时影响
    }
}

static void task_servo(void *arg)
{
    (void)arg;
    for (;;) {
        //EnableTorque(1, 1);            //上电控制
        WritePos(0, 500,  0, 100);    // 运动到 500 //
        vTaskDelay(pdMS_TO_TICKS(1500));
        servo_pos_now = scs_read_reg(15,SCS_POS_NOW,2);  //影响lvgl读数
        HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_GPIO_PIN);

        
        //EnableTorque(1, 1);
        WritePos(0, 3500, 0, 100);    
        vTaskDelay(pdMS_TO_TICKS(1500));
        servo_pos_now = scs_read_reg(15,SCS_POS_NOW,2);
        HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_GPIO_PIN);
    
       
    }
}
/**
 * @brief LED 闪烁 N 次（Active-LOW，闪一下 = on 100ms + off 150ms）
 * 用于诊断哪一步初始化卡住
 */
static void diag_blink(uint32_t n)
{
    for (uint32_t i = 0; i < n; i++) {
        HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_RESET); /* ON  */
        HAL_Delay(120);
        HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_SET);   /* OFF */
        HAL_Delay(180);
    }
    HAL_Delay(400); /* 间隔分隔不同组 */
}

/* ======================== 飞特舵机硬件接口实现 ======================== */

extern UART_HandleTypeDef huart1;

/**
 * @brief 舵机串口发送 (半双工)
 */
void ftUart_Send(uint8_t *nDat, int nLen)
{
    /* 发送期间关闭接收器，防止回显导致 ORE */
    CLEAR_BIT(huart1.Instance->CR1, USART_CR1_RE);

    /* HAL 阻塞发送 (内部已等 TXE + TC) */
    HAL_UART_Transmit(&huart1, nDat, nLen, 100);

    /* 重开接收器，同时清除硬件标志和 HAL 错误码 */
    __HAL_UART_CLEAR_FLAG(&huart1, UART_CLEAR_OREF | UART_CLEAR_NEF | UART_CLEAR_FEF);
    huart1.ErrorCode = HAL_UART_ERROR_NONE;   /* 必须重置，否则 HAL_UART_Receive 会因该字段非零直接返回 HAL_ERROR */
    SET_BIT(huart1.Instance->CR1, USART_CR1_RE);
}

/**
 * @brief 舵机串口接收 (半双工)
 */
int ftUart_Read(uint8_t *nDat, int nLen)
{
    /* 清硬件标志 + HAL 错误码，否则 HAL_UART_Receive 内部的
     * UART_WaitOnFlagUntilTimeout 会检测到残留 ORE 并返回 HAL_ERROR */
    __HAL_UART_CLEAR_FLAG(&huart1, UART_CLEAR_OREF | UART_CLEAR_NEF | UART_CLEAR_FEF);
    huart1.ErrorCode = HAL_UART_ERROR_NONE;

    if (HAL_OK != HAL_UART_Receive(&huart1, nDat, nLen, 100)) {
        return 0;
    }
    return nLen;
}

/**
 * @brief 总线延时 (半双工收发切换用)
 */
void ftBus_Delay(void)
{
    vTaskDelay(pdMS_TO_TICKS(2));   /* FreeRTOS 任务延时，替换 HAL_Delay */
    __HAL_UART_CLEAR_FLAG(&huart1, UART_CLEAR_OREF | UART_CLEAR_NEF | UART_CLEAR_FEF);
    huart1.ErrorCode = HAL_UART_ERROR_NONE;
    while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE)) {
        (void)huart1.Instance->RDR;
    }
}

/* ======================== 系统函数声明 ======================== */
void SystemClock_Config(void);
static void MPU_Config(void);

/* ======================== 主函数 ======================== */

int main(void)
{
    /* H750 必须顺序：MPU → Cache → HAL_Init → 时钟 */
    MPU_Config();          /* 1. MPU配置SDRAM区(0xC0000000)，必须在DCache前 */
    SCB_EnableICache();    /* 2. 使能指令缓存 */
    SCB_EnableDCache();    /* 3. 使能数据缓存 */

    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART1_UART_Init();

    /* PLL3 配置：LTDC 像素时钟 = HSE/25 * 330 / 10 = 33MHz
     * 必须在 MX_GPIO_Init() 之后才配置，这样 Error_Handler 里 LED 才能闪烁 */
    {
        RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
        PeriphClkInitStruct.PeriphClockSelection  = RCC_PERIPHCLK_LTDC | RCC_PERIPHCLK_FMC;
        PeriphClkInitStruct.FmcClockSelection     = RCC_FMCCLKSOURCE_D1HCLK;
        PeriphClkInitStruct.PLL3.PLL3M      = 25;
        PeriphClkInitStruct.PLL3.PLL3N      = 330;
        PeriphClkInitStruct.PLL3.PLL3P      = 2;
        PeriphClkInitStruct.PLL3.PLL3Q      = 2;
        PeriphClkInitStruct.PLL3.PLL3R      = 10;
        PeriphClkInitStruct.PLL3.PLL3RGE    = RCC_PLL3VCIRANGE_0;
        PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOMEDIUM;
        PeriphClkInitStruct.PLL3.PLL3FRACN  = 0;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
            Error_Handler(); /* 这时 LED GPIO 已初始化，能看到快闪 */
        }
    }

    //NOTE  该部分函数应封装
    /*
     * ===== LED 诊断闪烁规则 =====
     * 闪 1 次 = 已到达屏幕初始化前（系统基本 OK）
     * 闪 2 次 = MX_FMC_Init 完成
     * 闪 3 次 = MX_LTDC_Init 完成
     * 闪 4 次 = Touch_Init 完成
     * 闪 5 次 = LVGL 全部初始化完成，进入 while(1)
     * 如果 LED 卡住在某次闪烁之前：就是那一步卡住了
     */
    diag_blink(1);   /* 已过：SystemClock / GPIO / UART 初始化 */

    /* ---------- 屏幕 + LVGL 初始化 ---------- */
    MX_FMC_Init();             /* SDRAM 初始化（必须在 LTDC 之前） */
    diag_blink(2);   /* 已过：MX_FMC_Init */

    MX_LTDC_Init();            /* LTDC + RGB LCD 初始化 */
    diag_blink(3);   /* 已过：MX_LTDC_Init */

    touch_ok = (Touch_Init() == 0);  /* GT9XX 触摸屏初始化，0=成功 */
    diag_blink(4);   /* 已过：Touch_Init */
    
    lv_init();                 /* 初始化 LVGL */
    lv_port_disp_init();       /* 注册显示驱动（双帧缓冲，SDRAM） */
    lv_port_indev_init();      /* 注册触摸输入驱动 */
    //lv_demo_benchmark();
   
    diag_blink(5);   /* 已过：全部初始化，进入 while(1) */
    
    
    /* ---------- 飞特舵机初始化 ---------- */
    setEnd(1);     // SCSCL 系列: 大端存储
    setLevel(1);   // 关闭应答等待 (舵机仍会发应答包，MCU不读它，避免Ack超时卡死)


    //warning:启动时应创建相应的任务
    xTaskCreate(task_lvgl,  "lvgl",  1024, NULL, 2, NULL);
    xTaskCreate(task_servo, "servo",  512, NULL, 3, NULL); /* 高于 lvgl，但自身大部分时间在 vTaskDelay 里睡眠 */
    
    
    
    vTaskStartScheduler();

    /* 正常不会到这里 */
    for (;;) {}
}
/* ======================== 时钟配置 (HSE 25MHz → PLL → 480MHz) ========================
 *   HSE = 25MHz (反客FK750M5-XBH6 板载晶振)
 *   PLL1: M=5 → 5MHz VCI, N=192 → VCO=960MHz, P=2 → SYSCLK=480MHz
 *   HCLK   = 480/2  = 240 MHz
 *   APB1/2/3/4 = 240/2 = 120 MHz
 *   Flash Latency = 4WS (VOS0, 240MHz AXI)
 * ==================================================================================== */

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* 电源配置: LDO 供电 */
    HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

    /* 先配 VOS1, 稳定后再提升到 VOS0 */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

    /* 提升到 VOS0 (500MHz 以内使用) */
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);
    while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

    /* PLL1 配置 */
    RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState            = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM            = 5;      /* 25/5 = 5MHz VCI */
    RCC_OscInitStruct.PLL.PLLN            = 192;    /* 5*192 = 960MHz VCO */
    RCC_OscInitStruct.PLL.PLLP            = 2;      /* 960/2 = 480MHz SYSCLK */
    RCC_OscInitStruct.PLL.PLLQ            = 2;      /* 960/2 = 480MHz */     
    RCC_OscInitStruct.PLL.PLLR            = 2;      /* 960/2 = 480MHz */
    RCC_OscInitStruct.PLL.PLLRGE          = RCC_PLL1VCIRANGE_2;  /* 4~8MHz VCI */
    RCC_OscInitStruct.PLL.PLLVCOSEL       = RCC_PLL1VCOWIDE;     /* 192~960MHz VCO */
    RCC_OscInitStruct.PLL.PLLFRACN        = 0;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /* 总线时钟配置 */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK   | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1  | RCC_CLOCKTYPE_PCLK2
                                | RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
    RCC_ClkInitStruct.SYSCLKSource    = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider  = RCC_SYSCLK_DIV1;   /* SYSCLK = 480MHz */
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV2;     /* HCLK   = 240MHz */
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;     /* APB3   = 120MHz */
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;     /* APB1   = 120MHz */
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;     /* APB2   = 120MHz */
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;     /* APB4   = 120MHz */
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
        Error_Handler();
    }
    /* PLL3 在 main() 里配置，必须在 MX_GPIO_Init() 之后调用（不在这里！） */
}

/* ======================== MPU 配置 ========================
 * 为外部SDRAM区域(0xC0000000, 32MB)设置Cacheable/Write-Through属性
 * 即使本工程不主动使用SDRAM，也需配置以防DCache对该区域产生误操作
 * (反客FK750M5-XBH6 板载 32MB 16bit SDRAM)
 * ========================================================== */
static void MPU_Config(void)
{
    MPU_Region_InitTypeDef MPU_InitStruct = {0};

    HAL_MPU_Disable();

    /* SDRAM区域：Cacheable, Write-Through, Not Bufferable */
    MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
    MPU_InitStruct.Number           = MPU_REGION_NUMBER1;
    MPU_InitStruct.BaseAddress      = 0xC0000000;
    MPU_InitStruct.Size             = MPU_REGION_SIZE_32MB;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;
    MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/* ======================== 错误处理 ======================== */

void Error_Handler(void)
{
    /* LED 快闪 20 次 → 告知用户这里是 Error_Handler（不是调用所指向的初始化卡住），然后软件复位 */
    for (int i = 0; i < 20; i++) {
        HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_RESET);
        HAL_Delay(50);
        HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_SET);
        HAL_Delay(50);
    }
    NVIC_SystemReset(); /* 复位后重新运行，可连续观察闪烁规律 */
}

/* ======================== FreeRTOS 钩子函数 ======================== */

/**
 * @brief 栈溢出钩子：LED 快闪提示，然后死循环（便于调试定位）
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)xTask;
    (void)pcTaskName;
    /* 栈溢出是致命错误，不建议复位（可能掩盖问题），直接死循环便于调试 */
    __disable_irq();
    while (1) {}
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    (void)file;
    (void)line;
}
#endif
