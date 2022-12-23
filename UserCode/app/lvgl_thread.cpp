#include "lvgl_thread.hpp"
#include <exception>

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

void LvglThread::StartThread()
{
    if (mutex == nullptr) {
        mutex = xSemaphoreCreateRecursiveMutex();
        if (mutex == nullptr) {
            throw StrException("Unable to create mutex.");
        }
    } else {
        throw StrException("mutex != nullptr.");
    }

    if (thread_handle == nullptr) {
        auto result = xTaskCreate(thread_entry, "lvgl_thread", 512, this, osPriorityBelowNormal, thread_handle);
        if (result != pdPASS) {
            throw StrException("Unable to create lvgl_thread.");
        }
    } else {
        throw StrException("thread_handle != nullptr.");
    }
}

void LvglThread::thread_entry(void *argument)
{
    auto lvgl_thread = (LvglThread *)argument;

    lv_init();
    lv_port_disp_init();

    uint32_t PreviousWakeTime = xTaskGetTickCount();

    for (;;) {
        lvgl_thread->Lock();
        lv_task_handler();
        lvgl_thread->Unlock();

        vTaskDelayUntil(&PreviousWakeTime, 5);
    }
}
