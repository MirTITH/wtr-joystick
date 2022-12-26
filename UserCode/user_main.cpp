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
#include "component/fmt/ranges.h"
#include "wtr_log.hpp"
#include "lvgl_thread.hpp"
#include "fatfs.h"
#include "high_precision_time.h"

using namespace std;

uint8_t work_buff[16384];

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

    StartLvglMainThread();

    vTaskDelay(3000);

    printf("Start test\n");

    // auto result = f_mkfs(SDPath, FM_FAT32, 0, work_buff, sizeof(work_buff));

    // printf("f_mkfs: %d\n", result);

    for (int i = 0; i < sizeof(work_buff); i++) {
        work_buff[i] = i % 255;
    }

    auto retSD = f_mount(&SDFatFS, (TCHAR const *)SDPath, 1);
    if (retSD == FR_OK) {
        printf("Filesystem mount ok, now you can read/write files.\r\n");
        // 创建或者打开文件 SD_Card_test.txt
        retSD = f_open(&SDFile, "SD_Card_test.txt", FA_OPEN_ALWAYS | FA_WRITE);
        if (retSD == FR_OK) {
            printf("open/create SD_Card_test.txt OK, write data to it.\r\n");

            // Move to end of the file to append data
            retSD = f_lseek(&SDFile, f_size(&SDFile));
            if (retSD == FR_OK) {
                UINT bytesWritten;
                auto startTime = HPT_GetUs();
                f_write(&SDFile, work_buff, sizeof(work_buff), &bytesWritten);
                auto endTime = HPT_GetUs();
                printf("Write data to file OK, written bytes: %d\r\n", bytesWritten);
                printf("Time cost: %lu us. Speed: %lf KBytes / s.\r\n", endTime - startTime, 1000.0 * sizeof(work_buff) / (endTime - startTime));
            } else {
                printf("!! File Write error: (%d)\n", retSD);
            }

            f_close(&SDFile);
            f_open(&SDFile, "SD_Card_test.txt", FA_OPEN_ALWAYS | FA_READ);

            printf("Total size: %llu\r\n", f_size(&SDFile));

            memset(work_buff, 0, sizeof(work_buff));

            printf("Before read: \r\n");

            for (int i = 0; i < 10; i++) {
                printf("%d ", work_buff[i]);
            }

            printf("\r\n");

            if (retSD == FR_OK) {
                auto startTime = HPT_GetUs();
                UINT bytesRead;
                f_read(&SDFile, work_buff, sizeof(work_buff), &bytesRead);
                auto endTime = HPT_GetUs();
                printf("Read data OK, read bytes: %d\r\n", bytesRead);
                printf("Time cost: %lu us. Speed: %lf KBytes / s.\r\n", endTime - startTime, 1000.0 * sizeof(work_buff) / (endTime - startTime));
                for (int i = 0; i < sizeof(work_buff); i++) {
                    if (work_buff[i] != i % 255) {
                        printf("Read data wrong!\n");
                        break;
                    }
                }
            }
            /* close file */
            f_close(&SDFile);
        } else {
            printf("!! open/Create file SD_Card_test.txt Fail.\r\n");
        }
    } else {
        printf("!! SDcard mount filesystem error。(%d)\r\n", retSD);
    }

    // // 删除当前线程
    // vTaskDelete(nullptr);

    uint32_t PreviousWakeTime = xTaskGetTickCount();

    for (;;) {
        // Log() << 1.0055 << endl;
        // fmt::print("Hello\n");
        wtrDebug() << "Time:" << PreviousWakeTime << endl;
        vTaskDelayUntil(&PreviousWakeTime, 1000);
    }
}
