#include "lvgl_thread.hpp"
#include <exception>
#include <string>
#include "screen_console.hpp"

SemaphoreHandle_t LvglMutex       = nullptr;
static TaskHandle_t thread_handle = nullptr;

class StrException : public std::exception
{
private:
    const char *_str;

public:
    StrException(const char *str)
        : _str(str){};

    const char *what() const noexcept
    {
        return _str;
    }
};

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
    return xSemaphoreTake(LvglMutex, block_time);
}

BaseType_t LvglUnlock()
{
    return xSemaphoreGive(LvglMutex);
}

void StartLvglMainThread()
{
    if (LvglMutex == nullptr) {
        LvglMutex = xSemaphoreCreateMutex();
        if (LvglMutex == nullptr) {
            throw StrException("Unable to create mutex.");
        }
    } else {
        throw StrException("mutex != nullptr.");
    }

    if (thread_handle == nullptr) {
        auto result = xTaskCreate(LvglMainThreadEntry, "lvgl_thread", 512, nullptr, osPriorityBelowNormal, &thread_handle);
        if (result != pdPASS) {
            throw StrException("Unable to create lvgl_thread.");
        }
    } else {
        throw StrException("thread_handle != nullptr.");
    }
}
