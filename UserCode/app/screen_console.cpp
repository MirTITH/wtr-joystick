/**
 * @file screen_console.cpp
 * @author X. Y.
 * @brief
 * @version 0.1
 * @date 2022-12-23
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "screen_console.hpp"
#include "lvgl_thread.hpp"
#include <string>

using namespace std;

/* void ScreenConsoleThread(void *argument)
{
    (void)argument;

    // ScreenConsoleQueue = xQueueCreate(64, sizeof(char));

    LvglLock();
    text_area = lv_textarea_create(lv_scr_act());
    lv_style_init(&style);

    lv_style_set_height(&style, lv_pct(80));
    lv_style_set_width(&style, lv_pct(95));
    lv_style_set_align(&style, LV_ALIGN_BOTTOM_MID);

    lv_obj_add_style(text_area, &style, 0);
    lv_textarea_set_cursor_click_pos(text_area, false);
    LvglUnlock();

    // for (;;) {
    //     char receivedChar;
    //     if (xQueueReceive(ScreenConsoleQueue, &receivedChar, osWaitForever) == pdTRUE) {
    //         LvglLock();
    //         lv_textarea_add_char(text_area, receivedChar);
    //         LvglUnlock();
    //     }
    // }
} */

// void StartScreenConsoleThread()
// {
//     xTaskCreate(ScreenConsoleThread, "ScreenConsole", 8192, nullptr, osPriorityBelowNormal, nullptr);
// }

void ScreenConsole::Init()
{
    LvglLock();
    text_area = lv_textarea_create(lv_scr_act());
    lv_style_init(&style);

    lv_style_set_height(&style, lv_pct(80));
    lv_style_set_width(&style, lv_pct(95));
    lv_style_set_align(&style, LV_ALIGN_BOTTOM_MID);

    lv_obj_add_style(text_area, &style, 0);
    lv_textarea_set_cursor_click_pos(text_area, false);
    LvglUnlock();
}

void ScreenConsole::AddText(const string str)
{
    if (text_area != nullptr) {
        LvglLock();
        lv_textarea_add_text(text_area, str.c_str());
        LvglUnlock();
    }
}
