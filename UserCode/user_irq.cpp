#include "user_irq.hpp"
#include "lvgl.h"
#include "FreeRTOS.h"

void vApplicationTickHook(void)
{
    lv_tick_inc(1000 / configTICK_RATE_HZ);
}

// void DMA_XferCpltCallback(DMA_HandleTypeDef *hdma)
// {
//     extern lv_disp_drv_t disp_drv;
//     if (hdma->Instance == hdma_memtomem_dma1_stream0.Instance) {
//         lv_disp_flush_ready(&disp_drv);
//     }
// }
