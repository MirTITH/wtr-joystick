/**
 * @file stm32_st7796.hpp
 * @author X. Y.
 * @brief
 * @version 0.1
 * @date 2023-03-31
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include "main.h"
#include "lcd_io_fmc.hpp"

class LCD_ST7796 : public LcdIoFmc
{
public:
    enum class Rotation {
        portrait,
        landscape,
        inverse_portrait,
        inverse_landscape
    };

    enum class ColorOrder {
        BGR,
        RGB
    };

private:
    ColorOrder _colorOrder = ColorOrder::BGR;
    Rotation _rotation     = Rotation::landscape;

    void delay(uint32_t delay_ms);

public:
    const int Width  = 320;    // Physical width
    const int Height = 480;    // Physical height
    int _width       = Width;  // becasue we can change these later if needed
    int _height      = Height; // eg. in setRotation()

    void initializeDisplay(void);        // resets the display
    void startDisplay(void);             // update config registers with default values
    void setRotation(Rotation rotation); // set rotation of the screen
    void setColorOrder(ColorOrder color_order);
    ColorOrder getColorOrder() const;
    void updateRotationAndColorOrder();
    void invertDisplay(bool i); // inverts colors
    void displayOn(void);       // turns on the display
    void displayOff(void);      // turns of the display
    // void openWindow(int x0, int y0, int x1, int y1); // opens a custom pixel window with X, Y, width and height
    // void windowData(uint16_t);                                                                                 // data values for custom window
    // void windowData(uint16_t *c, int len);                                                                     // data values as array, and count for custom window
    void writeData(uint8_t);                            // writes 8-bit data to bus
    void writeCommand(uint8_t);                         // writes 8-bit command to bus
    void writeData16(uint16_t);                         // writes 16-bit data to 8-bit bus
    void writeData8(uint8_t);                           // writes any 8-bit data to 8-bit bus
    void setAddrWindow(int x0, int y0, int x1, int y1); // opens a custom pixel window with X1, Y1, X2, Y2
    int16_t getScanline();
    // void fillScreen(uint16_t);                                                                                 // fills the entire screen with a 16-bit color
};
