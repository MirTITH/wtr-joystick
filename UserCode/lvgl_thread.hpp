#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "semphr.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "cmsis_os.h"

BaseType_t LvglLock(TickType_t block_time = portMAX_DELAY);
BaseType_t LvglUnlock();

void StartLvglMainThread();

#ifdef __cplusplus
}
#endif


