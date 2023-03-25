/**
 * @file screen_console.hpp
 * @author X. Y.
 * @brief
 * @version 0.1
 * @date 2022-12-23
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "cmsis_os.h"
#include "lvgl_thread.hpp"
#include <string>

class ScreenConsole
{
private:
    lv_obj_t *text_area = nullptr;
    lv_style_t style;

public:
    void Init();
    void AddText(const std::string str);
};

// #ifdef __cplusplus
// extern "C" {
// #endif

// void StartScreenConsoleThread();

// #ifdef __cplusplus
// }
// #endif
