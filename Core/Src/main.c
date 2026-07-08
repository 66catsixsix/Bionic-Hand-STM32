#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "SCServo.h"
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
#include "stdlib.h"
#include "FreeRtos_task.h"
#include "flex.h"
#include "Finger.h"
#include "test_tasks.h"
#include "calib_tasks.h"
#include "EMG.h"
#if ENABLE_DHT11_TASK
#include "dht11.h"
#endif

#define LVGL_ONLY_TEST_MODE 0

uint8_t touch_ok = 0;
void flex_task(void* arg);
void flex_calibration_task(void * arg);
void Finger_Map_Update(void* arg);
//GlobalVar
volatile int16_t servo_pos_now = 0;
volatile int16_t  angle_now = 0;
extern int16_t  Step_Angle_Update(int16_t Angle_Set);
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;

extern uint8_t rx_buf[];
extern uint8_t rx_index ;
extern uint8_t tx_buf[];

extern  volatile int16_t angle_flag;  //舵机中断标志为
extern  volatile int16_t  angle_value;


/* ======================== 飞特舵机硬件接口实现 ======================== */

void ftUart_Send(uint8_t *nDat, int nLen)
{

    CLEAR_BIT(huart1.Instance->CR1, USART_CR1_RE);

    /* HAL 阻塞发送 (内部已等 TXE + TC) */
    HAL_UART_Transmit(&huart1, nDat, nLen, 100);

    __HAL_UART_CLEAR_FLAG(&huart1, UART_CLEAR_OREF | UART_CLEAR_NEF | UART_CLEAR_FEF);
    huart1.ErrorCode = HAL_UART_ERROR_NONE;   /* 必须重置，否则 HAL_UART_Receive 会因该字段非零直接返回 HAL_ERROR */
    SET_BIT(huart1.Instance->CR1, USART_CR1_RE);
}


int ftUart_Read(uint8_t *nDat, int nLen)
{
    __HAL_UART_CLEAR_FLAG(&huart1, UART_CLEAR_OREF | UART_CLEAR_NEF | UART_CLEAR_FEF);
    huart1.ErrorCode = HAL_UART_ERROR_NONE;

    if (HAL_OK != HAL_UART_Receive(&huart1, nDat, nLen, 100)) {
        return 0;
    }
    return nLen;
}


void ftBus_Delay(void)
{
    vTaskDelay(pdMS_TO_TICKS(2));  
    __HAL_UART_CLEAR_FLAG(&huart1, UART_CLEAR_OREF | UART_CLEAR_NEF | UART_CLEAR_FEF);
    huart1.ErrorCode = HAL_UART_ERROR_NONE;
    while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE)) {
        (void)huart1.Instance->RDR;
    }
}

/* ======================== 系统函数声明 ======================== */
void SystemClock_Config(void);
static void MPU_Config(void);


int main(void)
{
    MPU_Config();
    SCB_EnableICache();
    SCB_EnableDCache(); 
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
#if ENABLE_DHT11_TASK
    dht11_init();
#endif
#if !LVGL_ONLY_TEST_MODE
    flex_init();
    EMG_Init();
#endif
    MX_USART1_UART_Init();
    MX_USART3_UART_Init();
    __HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE); 
     HAL_NVIC_SetPriority(USART3_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
    HAL_UART_Receive_IT(&huart3, &rx_buf[rx_index], 1);
    

    {
        RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
        PeriphClkInitStruct.PeriphClockSelection  = RCC_PERIPHCLK_LTDC | RCC_PERIPHCLK_FMC;
        PeriphClkInitStruct.FmcClockSelection     = RCC_FMCCLKSOURCE_D1HCLK;
        PeriphClkInitStruct.PLL3.PLL3M = 25;
        PeriphClkInitStruct.PLL3.PLL3N = 330;
        PeriphClkInitStruct.PLL3.PLL3P = 2;
        PeriphClkInitStruct.PLL3.PLL3Q = 2;
        PeriphClkInitStruct.PLL3.PLL3R = 10;
        PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_0;
        PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOMEDIUM;
        PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
            Error_Handler();
        }
    }

    #if !LVGL_ONLY_TEST_MODE
    MX_FMC_Init();         
    MX_LTDC_Init();         

    touch_ok = (Touch_Init() == 0); 

    {
        char touch_buf[32];
        int len = sprintf(touch_buf, "Touch Init: %s\r\n", touch_ok ? "OK" : "FAIL");
        HAL_UART_Transmit(&huart3, (uint8_t *)touch_buf, len, 100);
    }

    lv_init();              
    lv_port_disp_init();       
    lv_port_indev_init();      
    //lv_demo_benchmark();
    #endif
   

#if !LVGL_ONLY_TEST_MODE
    setEnd(1);     
    setLevel(1);
    Hand_Init();

    //xTaskCreate(flex_calibration_task,"flex_calibration",512,NULL,2,NULL); //校准
    //xTaskCreate(Finger_Map_Update,"Finger_Map_Update",512,NULL,3,NULL);
#endif
    #if !LVGL_ONLY_TEST_MODE
    xTaskCreate(task_lvgl,  "lvgl",  1024, NULL, 3, NULL);
    #endif
#if !LVGL_ONLY_TEST_MODE
    xTaskCreate(task_touch, "touch", 512,  NULL, 3, NULL);
#endif

    //xTaskCreate(flex_task, "flex",  512, NULL, 3, NULL); //flex传感器测试
    //xTaskCreate(task_servo, "servo",  512, NULL, 3, NULL); //舵机串口测试
    //xTaskCreate(task_usart3, "usart3", 512, NULL, 3, NULL); //单次舵机发送测试
    //xTaskCreate(id_change_task,"change_motor_id",512,NULL,3,NULL);
    //xTaskCreate(id_total_task,"total_task",512,NULL,3,NULL);
    
    
    /*test_tasks*/
    //xTaskCreate(test_single_flex_task, "test_flex", 512, NULL, 3, NULL);
    //xTaskCreate(test_pa0_index_root_task, "pa0_index", 512, NULL, 3, NULL);
    //xTaskCreate(flex_calib_task, "flex_calib", 512, NULL, 2, NULL);
    //xTaskCreate(flex_debug_task,"flex_debug_task",512,NULL,3,NULL);
    //xTaskCreate(test_index_event_recorder_task, "idx_evt", 1024, NULL, 2, NULL);


#if ENABLE_DHT11_TASK
    xTaskCreate(dht11_test,"dht11test",512,NULL,3,NULL);
#endif

#if !LVGL_ONLY_TEST_MODE
    xTaskCreate(Finger_All_Control_TASK, "finger_all", 768, NULL, 3, NULL);
    xTaskCreate(EMG_TASK,"EMG_TASK",512,NULL,3,NULL);
#endif
    //xTaskCreate(flex_all_calib_task, "flex_all_calib", 1024, NULL, 4, NULL);  //标定完成会把calib_ok置1
    

    vTaskStartScheduler();

    /*！！！ //xTaskCreate(id_change_task,"change_motor_id",512,NULL,3,NULL); //ID修改*/

    for (;;) {}
}


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
