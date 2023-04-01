#include "lcd_io_fmc.hpp"
#include "tim.h"

void LcdIoFmc::WriteCmd8(uint8_t cmd)
{
    *LCD_COMMAND_ADDRESS = cmd;
}

void LcdIoFmc::WriteCmd16(uint16_t cmd)
{
    *LCD_COMMAND_ADDRESS = cmd;
}

void LcdIoFmc::WriteData8(uint8_t data)
{
    *LCD_DATA_ADDRESS = data;
}

void LcdIoFmc::WriteData16(uint16_t data)
{
    *LCD_DATA_ADDRESS = data;
}

void LcdIoFmc::SetBacklight(uint32_t lightness)
{
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, lightness);
}

void LcdIoFmc::LcdInit()
{
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    SetBacklight(100);
    HardReset();
}

void LcdIoFmc::HardReset()
{
    HAL_GPIO_WritePin(RESET_GPIOX, RESET_GPIO_PIN, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(RESET_GPIOX, RESET_GPIO_PIN, GPIO_PIN_SET);
}
