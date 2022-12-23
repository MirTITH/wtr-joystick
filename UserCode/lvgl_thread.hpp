#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "semphr.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "cmsis_os.h"

class LvglMain
{
public:
    // 获取单例
    static LvglMain &GetInstance()
    {
        // 需要 c++11 及以上标准以保证线程安全
        static LvglMain instance;
        return instance;
    }

    /**
     * @brief 启动 lvgl 主线程
     *
     */
    void StartThread();

    /**
     * @brief 加锁，可递归加锁。调用 lvgl 函数前一定要先加锁，防止多线程同时操作
     *
     * @param block_time 加锁等待时间
     * @return 如果加锁成功，返回 pdTRUE。如果超时，锁不可用，则返回 pdFALSE。
     */
    auto Lock(TickType_t block_time = portMAX_DELAY)
    {
        return xSemaphoreTakeRecursive(mutex, block_time);
    }

    /**
     * @brief 解锁，需要与加锁函数成对出现。调用完 lvgl 函数后要解锁，让其他线程能够调用。
     */
    auto Unlock()
    {
        return xSemaphoreGiveRecursive(mutex);
    }

private:
    SemaphoreHandle_t mutex     = nullptr;
    TaskHandle_t thread_handle = nullptr;

    static void thread_entry(void *argument);

    // 实现单例模式
    LvglMain(){};
    ~LvglMain()                           = default;
    LvglMain(const LvglMain &)            = delete;
    LvglMain &operator=(const LvglMain &) = delete;
};

#ifdef __cplusplus
}
#endif
