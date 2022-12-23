#include "user_main.hpp"
#include "main.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include <cstring>
#include "freertos_usb_io.h"
#include <cstdlib>
#include <cstdio>
#include "usb_device.h"
#include "usb_reset.h"
#include "fmt/ranges.h"
#include "wtr_log.hpp"
#include "lvgl_thread.hpp"

using namespace std;

void SysInit()
{
    // 各区域bss段的开始结束地址（定义在 ld 文件中）
    extern uint32_t _sbss_d1, _sbss_d2, _sbss_d3, _sbss_itcm;
    extern uint32_t _ebss_d1, _ebss_d2, _ebss_d3, _ebss_itcm;

    /**
     * @brief 清理内存的函数
     * @param startPos 开始地址
     * @param endPos 结束地址
     */
    auto clearMem = [](void *startPos, void *endPos) {
        memset(startPos, 0, (uint32_t)endPos - (uint32_t)startPos);
    };

    // 清零各区域的 bss 段
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

    try {
        LvglThread::GetInstance().StartThread();
    } catch (const std::exception &e) {
        wtrErrorLine() << "Err. Start lvgl thread failed :" << e.what() << endl;
    } catch (...) {
        wtrErrorLine() << "Err. Start lvgl thread failed : Unknown reason." << endl;
    }

    // 删除当前线程
    // vTaskDelete(nullptr);

    uint32_t PreviousWakeTime = xTaskGetTickCount();

    for (;;) {
        // Log() << 1.0055 << endl;
        // fmt::print("Hello\n");
        wtrDebug() << "Time:" << PreviousWakeTime << endl;
        vTaskDelayUntil(&PreviousWakeTime, 1000);
    }
}
