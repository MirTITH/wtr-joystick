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

// extern const lv_font_t LXWKMono_16;

void ScreenConsoleThread(void *argument)
{
    (void)argument;

    // string str;
    int counter = 0;

    LvglLock();
    static auto text_area = lv_textarea_create(lv_scr_act());
    static lv_style_t style;
    lv_style_init(&style);
    lv_font_t *font = nullptr;
    // lv_font_t *font = lv_freetype_font_create("0:wtr-controller/fonts/LXGWWenKaiMonoGB-Regular.ttf", 20, LV_FREETYPE_FONT_STYLE_NORMAL);
    // lv_font_t *font = lv_freetype_font_create("0:wtr-controller/fonts/LXGWWenKaiMono-Bold.ttf", 20, LV_FREETYPE_FONT_STYLE_NORMAL);
    if (font != nullptr) {
        lv_style_set_text_font(&style, font);
    } else {
        lv_textarea_add_text(text_area, "Failed to load font\n");
    }

    lv_style_set_height(&style, lv_pct(95));
    lv_style_set_width(&style, lv_pct(95));
    lv_style_set_align(&style, LV_ALIGN_CENTER);

    lv_obj_add_style(text_area, &style, 0);
    lv_textarea_set_cursor_click_pos(text_area, false);
    LvglUnlock();

    for (;;) {

        if (counter < 50) {
            vTaskDelay(100);
            LvglLock();
            // str = "Hello world! Text area " + to_string(counter) + "\n";
            counter++;
            lv_textarea_add_text(text_area, "Hello, world! 你好，世界！( •̀ ω •́ )y");
            // lv_textarea_add_text(text_area, "Hello, world!");
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
    xTaskCreate(ScreenConsoleThread, "ScreenConsole", 8192, nullptr, osPriorityBelowNormal, nullptr);
}
