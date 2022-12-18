#include "user_main.hpp"
#include "main.h"
#include "cmsis_os.h"
#include <string.h>
#include "freertos_usb_io.h"
#include "stdio.h"
#include "usb_device.h"
#include "usb_reset.h"


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
    osDelay(500);

    for (;;) {
        printf("Hello\n");
        vTaskDelay(100);
    }
}
