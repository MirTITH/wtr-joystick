/**
 * @file micro_second.c
 * @author X. Y.
 * @brief 高精度时间库
 * @version 0.3
 * @date 2022-12-26
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "high_precision_time.h"

#define USE_FREERTOS // 使用 FreeRTOS 时请打开该宏定义

#ifdef USE_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#include "in_handle_mode.h"
#define HAL_TIMEBASE_htimx htim7 // Timebase Source
extern TIM_HandleTypeDef HAL_TIMEBASE_htimx;
#endif

uint32_t HPT_GetUs()
{
#ifdef USE_FREERTOS
    return uwTick * 1000 + HAL_TIMEBASE_htimx.Instance->CNT;
#else
    return uwTick * 1000 + (SysTick->LOAD - SysTick->VAL) / (SystemCoreClock / (1000 * 1000 / uwTickFreq));
#endif
}

void HPT_DelayUs(uint32_t us)
{
#ifdef USE_FREERTOS
    if (InHandlerMode()) {
        uint32_t beforeTaskDelay = HPT_GetUs();
        while ((HPT_GetUs()) < us + beforeTaskDelay) {
        }
    } else {
        uint32_t beforeTaskDelay = HPT_GetUs();
        if (us >= 1000000 / configTICK_RATE_HZ) {
            vTaskDelay(us / 1000);
        }
        while ((HPT_GetUs()) < us + beforeTaskDelay) {
        }
    }
#else
    uint32_t usStart = HPT_GetUs();
    while ((HPT_GetUs() - usStart) < us) {
    }
#endif // USE_FREERTOS
}

void HPT_DelayMs(uint32_t ms)
{
    HPT_DelayUs(1000 * ms);
}

uint64_t HPT_GetNs()
{
#ifdef USE_FREERTOS
    return (uint64_t)xTaskGetTickCount() * (1000000000 / configTICK_RATE_HZ) + (uint64_t)(SysTick->LOAD - SysTick->VAL) * (1000000 * configTICK_RATE_HZ) / configCPU_CLOCK_HZ;
#else
    return (uint64_t)uwTick * 1000000 + (uint64_t)(SysTick->LOAD - SysTick->VAL) * (1000000000 / uwTickFreq) / SystemCoreClock;
#endif
}
