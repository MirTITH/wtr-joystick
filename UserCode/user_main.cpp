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
#include "wtr_log.hpp"
#include "lvgl_thread.hpp"
// #include "fatfs.h"
#include "high_precision_time.h"
#include "stdio_CLI.h"
#include "screen_console.hpp"

using namespace std;

void SysInit()
{
    __HAL_RCC_D2SRAM1_CLK_ENABLE();
    __HAL_RCC_D2SRAM2_CLK_ENABLE();
    __HAL_RCC_D2SRAM3_CLK_ENABLE();
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

extern ScreenConsole screen_console;

void StartDefaultTask(void *argument)
{
    (void)argument;
    USB_Reset();
    MX_USB_DEVICE_Init();
    FreeRTOS_IO_Init();

    // vTaskDelay(200);
    // f_mount(&SDFatFS, (TCHAR const *)SDPath, 1);
    // vTaskDelay(200);
    StartLvglMainThread();

    CLI_Start();

    // uint8_t res;

    // res = mpu9250(argc, argv);
    // if (res == 0) {
    //     /* run success */
    // } else if (res == 1) {
    //     mpu9250_interface_debug_print("mpu9250: run failed.\n");
    // } else if (res == 5) {
    //     mpu9250_interface_debug_print("mpu9250: param is invalid.\n");
    // } else {
    //     mpu9250_interface_debug_print("mpu9250: unknown status code.\n");
    // }

    // MPU9250_Init();

    // for (;;) {
    //     int16_t AccData[3], GyroData[3], MagData[3];
    //     MPU9250_GetData(AccData, MagData, GyroData);

    //     printf("%08d;%08d;%08d;%08d;%08d;%08d;%08d;%08d;%08d\n",
    //            (int16_t)AccData[0], (int16_t)AccData[1], (int16_t)AccData[2],
    //            (int16_t)GyroData[0], (int16_t)GyroData[1], (int16_t)GyroData[2],
    //            (int16_t)MagData[0], (int16_t)MagData[1], (int16_t)MagData[2]);

    //     vTaskDelay(100);
    // }

    // 删除当前线程
    vTaskDelete(nullptr);

    // UINT readSize;

    // uint32_t totalReadSize = 0;

    // printf("Opening file\n");

    // printf("Opened file\n");

    // auto startTime = HPT_GetUs();
    // f_open(&SDFile, "0:wtr-controller/fonts/LXGWWenKai-Regular.ttf", FA_READ);

    // do {
    //     f_read(&SDFile, work_buff, sizeof(work_buff), &readSize);
    //     totalReadSize += readSize;
    // } while (readSize != 0);

    // f_close(&SDFile);

    // auto endTime = HPT_GetUs();

    // printf("totalReadSize=%lu\n", totalReadSize);
    // printf("time:%lu\n", endTime - startTime);
    // printf("speed:%f KB/s\n", (float)totalReadSize / (endTime - startTime) * 1000);

    // uint32_t PreviousWakeTime = xTaskGetTickCount();

    // extern uint32_t totalBytesRead;
    // uint32_t lastTotalBytesRead = totalBytesRead;

    // for (;;) {
    //     // Log() << 1.0055 << endl;
    //     // fmt::print("Hello\n");
    //     // wtrDebug() << "Time:" << PreviousWakeTime << endl;

    //     printf("totalBytesRead: %lu speed: %f KB/s\n", totalBytesRead, (float)(totalBytesRead - lastTotalBytesRead) / 2000);
    //     lastTotalBytesRead = totalBytesRead;
    //     vTaskDelayUntil(&PreviousWakeTime, 2000);
    // }
}
