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

extern const lv_font_t LXWKMono_16;

void ScreenConsoleThread(void *argument)
{
    (void)argument;

    // string str;
    int counter = 0;

    LvglLock();
    static auto text_area = lv_textarea_create(lv_scr_act());
    lv_obj_set_style_text_font(text_area, &LXWKMono_16, 0);
    lv_obj_set_height(text_area, lv_pct(95));
    lv_obj_set_width(text_area, lv_pct(95));
    lv_obj_align(text_area, LV_ALIGN_CENTER, 0, 0);
    lv_textarea_set_cursor_click_pos(text_area, false);
    LvglUnlock();

    for (;;) {

        if (counter < 50) {
            vTaskDelay(100);
            LvglLock();
            // str = "Hello world! Text area " + to_string(counter) + "\n";
            counter++;
            lv_textarea_add_text(text_area, "Hello, world! 你好，世界！（霞鹜文楷体）");
            lv_textarea_add_text(text_area, to_string(counter).c_str());
            lv_textarea_add_char(text_area, '\n');
            LvglUnlock();
        } else {
            vTaskDelay(10);
            LvglLock();
            lv_textarea_del_char(text_area);
            auto text = lv_textarea_get_text(text_area);
            LvglUnlock();
            if (text[0] == '\0') counter = 0;
        }
    }
}

void StartScreenConsoleThread()
{
    xTaskCreate(ScreenConsoleThread, "ScreenConsole", 512, nullptr, osPriorityBelowNormal, nullptr);
}
