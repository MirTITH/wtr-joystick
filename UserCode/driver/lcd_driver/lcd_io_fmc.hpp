#pragma once

#include "main.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "in_handle_mode.h"

class LcdIoFmc
{
private:
    /**
     * LCD_BASE_ADDRESS 要根据 Chip Select 修改为对应的值
     * NE1 0x60000000
     * NE2 0x64000000
     * NE3 0x68000000
     * NE4 0x6C000000
     * REGSELECT_BIT: for example: A18 pin -> 18
     */
    const uint32_t LCD_BASE = 0x60000000;

    // 背光控制引脚
    GPIO_TypeDef *const BL_GPIOX = GPIOC;
    const uint16_t BL_GPIO_PIN   = GPIO_PIN_6;
    const bool BL_ON_LEVEL       = true;

    // RESET 引脚
    GPIO_TypeDef *const RESET_GPIOX = LcdReset_GPIO_Port;
    const uint16_t RESET_GPIO_PIN   = LcdReset_Pin;

    SemaphoreHandle_t _sem = nullptr;

public:
    volatile uint16_t *const LCD_COMMAND_ADDRESS = (uint16_t *)LCD_BASE;
    volatile uint16_t *const LCD_DATA_ADDRESS    = (uint16_t *)(LCD_BASE + (0x4000000 - 2));

    LcdIoFmc()
    {
        _sem = xSemaphoreCreateBinary();
        xSemaphoreGive(_sem);
    }

    ~LcdIoFmc()
    {
        vSemaphoreDelete(_sem);
    }

    // void SemaphoreInit();
    // void SemaphoreDeinit();
    void SemaphoreTake(TickType_t tick = portMAX_DELAY)
    {
        if (InHandlerMode()) {
            xSemaphoreTakeFromISR(_sem, nullptr);
        } else {
            xSemaphoreTake(_sem, tick);
        };
    }

    void SemaphoreGive()
    {
        if (InHandlerMode()) {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            xSemaphoreGiveFromISR(_sem, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        } else {
            xSemaphoreGive(_sem);
        }
    };

    void InitBacklight();
    void HardReset();
    uint16_t ReadData();
    void WriteCmd8(uint8_t cmd);
    void WriteCmd16(uint16_t cmd);
    void WriteData8(uint8_t data);
    void WriteData16(uint16_t data);
    
    void SetBacklight(uint32_t lightness);
};
