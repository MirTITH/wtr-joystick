#include "main.h"
#include "lvgl.h"
#include "FreeRTOS.h"

#ifdef __cplusplus
extern "C" {
#endif

void vApplicationTickHook();
void HAL_GPIO_EXTI_Callback(uint16_t pin);
// void DMA_XferCpltCallback(DMA_HandleTypeDef *hdma);

#ifdef __cplusplus
}
#endif

void vApplicationTickHook(void)
{
    lv_tick_inc(1000 / configTICK_RATE_HZ);
}

/**
 * @brief     gpio exti callback
 * @param[in] pin is the gpio pin
 * @note      none
 */
void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
    switch (pin) {
        case GPIO_PIN_0:
            extern uint32_t MpuInterruptCount;
            MpuInterruptCount++;
            break;

        default:
            break;
    }
}

// void DMA_XferCpltCallback(DMA_HandleTypeDef *hdma)
// {
//     extern lv_disp_drv_t disp_drv;
//     if (hdma->Instance == hdma_memtomem_dma1_stream0.Instance) {
//         lv_disp_flush_ready(&disp_drv);
//     }
// }
