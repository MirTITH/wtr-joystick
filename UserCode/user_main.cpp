#include "user_main.hpp"
#include "main.h"
#include "cmsis_os.h"
#include <cstring>
#include "freertos_usb_io.h"
#include <cstdlib>
#include <cstdio>
#include "usb_device.h"
#include "usb_reset.h"
#include "fmt/ranges.h"
#include "wtr_log.hpp"
#include "STM32_ILI9481.hpp"

using namespace std;

LCD_ILI9481 LCD;

void SysInit()
{
    extern uint32_t _sbss_d1, _sbss_d2, _sbss_d3, _sbss_itcm;
    extern uint32_t _ebss_d1, _ebss_d2, _ebss_d3, _ebss_itcm;

    auto clearMem = [](void *startPos, void *endPos) {
        memset(startPos, 0, (uint32_t)endPos - (uint32_t)startPos);
    };

    clearMem(&_sbss_d1, &_ebss_d1);
    clearMem(&_sbss_d2, &_ebss_d2);
    clearMem(&_sbss_d3, &_ebss_d3);
    clearMem(&_sbss_itcm, &_ebss_itcm);
}

void StartDefaultTask(void *argument)
{
    (void)argument;
    USB_Reset();
    MX_USB_DEVICE_Init();
    FreeRTOS_IO_Init();

    LCD.initializeDisplay();

    for (;;) {
        Log() << 1.0055 << endl;
        // fmt::print("Hello\n");
        vTaskDelay(100);
    }
}
