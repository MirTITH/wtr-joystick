#include "lvgl_thread.hpp"
#include "screen_console.hpp"

static SemaphoreHandle_t LvglMutex;
static TaskHandle_t thread_handle = nullptr;

static void LvglMainThreadEntry(void *argument)
{
    (void)argument;
    lv_init();
    lv_port_disp_init();

    StartScreenConsoleThread();

    uint32_t PreviousWakeTime = xTaskGetTickCount();

    for (;;) {
        LvglLock();
        lv_task_handler();
        LvglUnlock();

        vTaskDelayUntil(&PreviousWakeTime, 5);
    }
}

BaseType_t LvglLock(TickType_t block_time)
{
    return xSemaphoreTakeRecursive(LvglMutex, block_time);
}

BaseType_t LvglUnlock()
{
    return xSemaphoreGiveRecursive(LvglMutex);
}

void StartLvglMainThread()
{
    // LvglMutex = xSemaphoreCreateRecursiveMutexStatic(&LvglMutexBuffer);
    LvglMutex = xSemaphoreCreateRecursiveMutex();
    xTaskCreate(LvglMainThreadEntry, "lvgl_thread", 2048, nullptr, osPriorityNormal, &thread_handle);
}
