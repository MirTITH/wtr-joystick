#include "user_irq.hpp"
#include "lvgl.h"
#include "FreeRTOS.h"
#include "mpu9250_mutex.h"

void vApplicationTickHook(void)
{
    lv_tick_inc(1000 / configTICK_RATE_HZ);
}

extern uint8_t (*g_gpio_irq)(void);

/**
 * @brief     gpio exti callback
 * @param[in] pin is the gpio pin
 * @note      none
 */
void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
    if (pin == GPIO_PIN_0) {
        /* run the callback in the mutex mode */
        mutex_irq(g_gpio_irq);
    }
}

// void DMA_XferCpltCallback(DMA_HandleTypeDef *hdma)
// {
//     extern lv_disp_drv_t disp_drv;
//     if (hdma->Instance == hdma_memtomem_dma1_stream0.Instance) {
//         lv_disp_flush_ready(&disp_drv);
//     }
// }
