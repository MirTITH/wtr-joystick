/**
 * @file stm32_st7796.cpp
 * @author X. Y.
 * @brief
 * @version 0.1
 * @date 2023-03-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "stm32_st7796.hpp"

#define ST7796S_NOP       0x00 // No Operation
#define ST7796S_SWRESET   0x01 // Software reset
#define ST7796S_RDDID     0x04 // Read Display ID
#define ST7796S_RDNUMED   0x05 // Read Number of the Errors on DSI
#define ST7796S_RDDST     0x09 // Read Display Status
#define ST7796S_RDDPM     0x0A // Read Display Power Mode
#define ST7796S_RDDMADCTL 0x0B // Read Display MADCTL
#define ST7796S_RDDCOLMOD 0x0C // Read Display Pixel Format
#define ST7796S_RDDIM     0x0D // Read Display Image Mode
#define ST7796S_RDDSM     0x0E // Read Display Signal Status
#define ST7796S_RDDSDR    0x0F // Read Display Self-Diagnostic Result
#define ST7796S_SLPIN     0x10 // Sleep In
#define ST7796S_SLPOUT    0x11 // Sleep Out
#define ST7796S_PTLON     0x12 // Partial Display Mode On
#define ST7796S_NORON     0x13 // Normal Display Mode On
#define ST7796S_INVOFF    0x20 // Display Inversion Off
#define ST7796S_INVON     0x21 // Display Inversion On
#define ST7796S_DISPOFF   0x28 // Display Off
#define ST7796S_DISPON    0x29 // Display On
#define ST7796S_CASET     0x2A // Column Address Set
#define ST7796S_RASET     0x2B // Row Address Set
#define ST7796S_RAMWR     0x2C // Memory Write
#define ST7796S_RAMRD     0x2E // Memory Read
#define ST7796S_PTLAR     0x30 // Partial Area
#define ST7796S_VSCRDEF   0x33 // Vertical Scrolling Definition
#define ST7796S_TEOFF     0x34 // Tearing Effect Line OFF
#define ST7796S_TEON      0x35 // Tearing Effect Line On
#define ST7796S_MADCTL    0x36 // Memory Data Access Control
#define ST7796S_VSCSAD    0x37 // Vertical Scroll Start Address of RAM
#define ST7796S_IDMOFF    0x38 // Idle Mode Off
#define ST7796S_IDMON     0x39 // Idle Mode On
#define ST7796S_COLMOD    0x3A // Interface Pixel Format
#define ST7796S_WRMEMC    0x3C // Write Memory Continue
#define ST7796S_RDMEMC    0x3E // Read Memory Continue
#define ST7796S_STE       0x44 // Set Tear ScanLine
#define ST7796S_GSCAN     0x45 // Get ScanLine
#define ST7796S_WRDISBV   0x51 // Write Display Brightness
#define ST7796S_RDDISBV   0x52 // Read Display Brightness Value
#define ST7796S_WRCTRLD   0x53 // Write CTRL Display
#define ST7796S_RDCTRLD   0x54 // Read CTRL value Display
#define ST7796S_WRCABC    0x55 // Write Adaptive Brightness Control
#define ST7796S_RDCABC    0x56 // Read Content Adaptive Brightness Control
#define ST7796S_WRCABCMB  0x5E // Write CABC Minimum Brightness
#define ST7796S_RDCABCMB  0x5F // Read CABC Minimum Brightness
#define ST7796S_RDFCS     0xAA // Read First Checksum
#define ST7796S_RDCFCS    0xAF // Read Continue Checksum
#define ST7796S_RDID1     0xDA // Read ID1
#define ST7796S_RDID2     0xDB // Read ID2
#define ST7796S_RDID3     0xDC // Read ID3

#define ST7796S_IFMODE    0xB0 // Interface Mode Control
#define ST7796S_FRMCTR1   0xB1 // Frame Rate Control (In Normal Mode/Full Colors)
#define ST7796S_FRMCTR2   0xB2 // Frame Rate Control 2 (In Idle Mode/8 colors)
#define ST7796S_FRMCTR3   0xB3 // Frame Rate Control 3(In Partial Mode/Full Colors)
#define ST7796S_DIC       0xB4 // Display Inversion Control
#define ST7796S_BPC       0xB5 // Blanking Porch Control
#define ST7796S_DFC       0xB6 // Display Function Control
#define ST7796S_EM        0xB7 // Entry Mode Set
#define ST7796S_PWR1      0xC0 // Power Control 1
#define ST7796S_PWR2      0xC1 // Power Control 2
#define ST7796S_PWR3      0xC2 // Power Control 3
#define ST7796S_VCMPCTL   0xC5 // VCOM Control
#define ST7796S_VCMOST    0xC6 // VCOM Offset Register
#define ST7796S_NVMADW    0xD0 // NVM Address/Data Write
#define ST7796S_NVMBPROG  0xD1 // NVM Byte Program
#define ST7796S_NVMSTRD   0xD2 // NVM Status Read
#define ST7796S_RDID4     0xD3 // Read ID4
#define ST7796S_PGC       0xE0 // Positive Gamma Control
#define ST7796S_NGC       0xE1 // Negative Gamma Control
#define ST7796S_DGC1      0xE2 // Digital Gamma Control 1
#define ST7796S_DGC2      0xE3 // Digital Gamma Control 2
#define ST7796S_DOCA      0xE8 // Display Output Ctrl Adjust
#define ST7796S_CSCON     0xF0 // Command Set Control
#define ST7796S_SPIRC     0xFB // SPI Read Control

#define TFT_NO_ROTATION   (ST7796S_MADCTL_MV)
#define TFT_ROTATE_90     (ST7796S_MADCTL_MX)
#define TFT_ROTATE_180    (ST7796S_MADCTL_MV | ST7796S_MADCTL_MX | ST7796S_MADCTL_MY)
#define TFT_ROTATE_270    (ST7796S_MADCTL_MY)

void LCD_ST7796::delay(uint32_t delay_ms)
{
    HAL_Delay(delay_ms - 1);
}

void LCD_ST7796::initializeDisplay(void)
{
    InitBacklight();
    startDisplay();
}

void LCD_ST7796::startDisplay(void)
{
    // SemaphoreInit();
    SemaphoreTake();
    writeCommand(ST7796S_SWRESET);
    delay(100);

    writeCommand(ST7796S_SLPOUT);
    delay(20);

    writeCommand(ST7796S_CSCON);
    writeData8(0xc3); // enable command 2 part I
    writeCommand(ST7796S_CSCON);
    writeData8(0x96); // enable command 2 part II

    writeCommand(ST7796S_COLMOD);
    writeData8(0x55); // 0x55:16bit/pixel, 0x66:18bit/pixel

    writeCommand(ST7796S_DIC);
    writeData8(0x01);

    writeCommand(ST7796S_EM);
    writeData8(0xc6);

    writeCommand(ST7796S_PWR2);
    writeData8(0x15);

    writeCommand(ST7796S_PWR3);
    writeData8(0xaf);

    writeCommand(ST7796S_VCMPCTL);
    writeData8(0x22);

    writeCommand(ST7796S_VCMOST);
    writeData8(0x00);

    writeCommand(ST7796S_DOCA);
    writeData8(0x40);
    writeData8(0x8a);
    writeData8(0x00);
    writeData8(0x00);
    writeData8(0x29);
    writeData8(0x19);
    writeData8(0xa5);
    writeData8(0x33);

    writeCommand(ST7796S_PGC);
    writeData8(0xF0);
    writeData8(0x04);
    writeData8(0x08);
    writeData8(0x09);
    writeData8(0x08);
    writeData8(0x15);
    writeData8(0x2F);
    writeData8(0x42);
    writeData8(0x46);
    writeData8(0x28);
    writeData8(0x15);
    writeData8(0x16);
    writeData8(0x29);
    writeData8(0x2D);

    writeCommand(ST7796S_NGC);
    writeData8(0xF0);
    writeData8(0x04);
    writeData8(0x09);
    writeData8(0x09);
    writeData8(0x08);
    writeData8(0x15);
    writeData8(0x2E);
    writeData8(0x46);
    writeData8(0x46);
    writeData8(0x28);
    writeData8(0x15);
    writeData8(0x15);
    writeData8(0x29);
    writeData8(0x2D);

    // 由于IC在横屏状态下无法垂直刷新屏幕，无法进行垂直同步，所以IC刷新率不重要了
    // writeCommand(ST7796S_BPC);
    // writeData8(141);   // VFP, Front porch of Number lines (Min=2,Typ=8)
    // writeData8(4);    // VBP, Back porch of Number lines (Min=2,Typ=4)
    // writeData8(0x00); // All zeros
    // writeData8(4);    // HBP, Dotclk number of horizontal back porch period (Min=4)

    // /**
    //  * Set frame rate
    //  * FrameRate = 10^7 / ((168 + RTNA + 32 * (15 - FRS))*(480 + VFP + VBP))
    //  */
    // uint8_t FRS  = 11; // FRS[3:0]: Sets the frame frequency of full color normal mode.
    // uint8_t DIVA = 0;  // DIVA [1:0] : division ratio for internal clocks when Normal mode.
    // uint8_t RTNA = 24; // RTNA [4:0] : RTNA[4:0] is used to set 1H (line) period of Normal mode at CPU interface.
    // writeCommand(ST7796S_FRMCTR1);
    // writeData8(((FRS & 0x0f) << 4) | (DIVA & 0x03));
    // writeData8(RTNA & 0x0f);

    writeCommand(ST7796S_NORON);

    writeCommand(ST7796S_WRCTRLD);
    writeData8(0x24);
    writeCommand(ST7796S_CSCON);
    writeData8(0x3C);
    writeCommand(ST7796S_CSCON);
    writeData8(0x69);

    SemaphoreGive();

    setRotation(Rotation::landscape);
    setColorOrder(ColorOrder::BGR);
    invertDisplay(true);

    displayOn();
}

void LCD_ST7796::setRotation(Rotation rotation)
{
    _rotation = rotation;
    switch (rotation) {
        case Rotation::portrait:
            _width  = Width;
            _height = Height;
            break;
        case Rotation::landscape:
            _width  = Height;
            _height = Width;
            break;
        case Rotation::inverse_portrait:
            _width  = Width;
            _height = Height;
            break;
        case Rotation::inverse_landscape:
            _width  = Height;
            _height = Width;
            break;

        default:
            break;
    }
    updateRotationAndColorOrder();
}

void LCD_ST7796::setColorOrder(ColorOrder color_order)
{
    _colorOrder = color_order;
    updateRotationAndColorOrder();
}

LCD_ST7796::ColorOrder LCD_ST7796::getColorOrder() const
{
    return _colorOrder;
}

void LCD_ST7796::updateRotationAndColorOrder()
{
    uint8_t madctrl_data;
    switch (_rotation) {
        case Rotation::portrait:
            madctrl_data = 0x80;
            break;

        case Rotation::landscape:
            madctrl_data = 0x20;
            break;

        case Rotation::inverse_portrait:
            madctrl_data = 0x40;
            break;

        case Rotation::inverse_landscape:
            madctrl_data = 0xe0;
            break;

        default:
            madctrl_data = 0x20;
            break;
    }

    switch (_colorOrder) {
        case ColorOrder::BGR:
            madctrl_data |= 0x08;
            break;
        case ColorOrder::RGB:
            madctrl_data |= 0x00;
            break;

        default:
            break;
    }

    SemaphoreTake();

    writeCommand(ST7796S_MADCTL);
    writeData8(madctrl_data);

    SemaphoreGive();
}

void LCD_ST7796::invertDisplay(bool i)
{
    SemaphoreTake();
    writeCommand(i ? ST7796S_INVON : ST7796S_INVOFF);
    SemaphoreGive();
}

void LCD_ST7796::displayOn(void)
{
    SemaphoreTake();
    writeCommand(ST7796S_DISPON); // set display on
    SemaphoreGive();
}

void LCD_ST7796::displayOff(void)
{
    SemaphoreTake();
    writeCommand(ST7796S_DISPOFF); // set display off
    SemaphoreGive();
}

// void LCD_ST7796::openWindow(int x0, int y0, int x1, int y1)
// {
//     setAddrWindow(x0, y0, x0 + x1 - 1, y0 + y1 - 1);
// }

void LCD_ST7796::writeData(uint8_t inputData)
{
    WriteData8(inputData);
}

void LCD_ST7796::writeCommand(uint8_t inputCommand)
{
    WriteCmd8(inputCommand);
}

// writes 16 bit data to the 8-bit bus

void LCD_ST7796::writeData16(uint16_t inputData)
{
    WriteData16(inputData);
}

//------------------------------------------------------------------------//
// writes 8-bit data to 8-bit bus

void LCD_ST7796::writeData8(uint8_t inputData)
{
    WriteData8(inputData);
}

void LCD_ST7796::setAddrWindow(int x0, int y0, int x1, int y1)
{
    writeCommand(ST7796S_CASET); // set col address - 4 param
    writeData8((x0 & 0xff00) >> 8);
    writeData8(x0 & 0xff);
    writeData8((x1 & 0xff00) >> 8);
    writeData8(x1 & 0xff);

    writeCommand(ST7796S_RASET); // set page address - 4 param
    writeData8((y0 & 0xff00) >> 8);
    writeData8(y0 & 0xff);
    writeData8((y1 & 0xff00) >> 8);
    writeData8(y1 & 0xff);

    writeCommand(ST7796S_RAMWR); // write memory start
}

int16_t LCD_ST7796::getScanline()
{
    // SemaphoreTake();
    uint16_t result;
    writeCommand(ST7796S_GSCAN);
    ReadData();
    result = (ReadData() & 0x00ff) << 8;
    result |= ReadData() & 0x00ff;
    // SemaphoreGive();

    return result;
}
