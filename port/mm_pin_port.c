/**
 * @file mm_gpio_port.c
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief
 * @version 0.1
 * @date 2024-03-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "mm_pin.h"
#include "Chip.h"
void mm_pin_mode(uint32_t pin, uint8_t mode)
{
    if (mode == PIN_MODE_OUTPUT)
    {
        Chip_GPIO_SetPinDIROutput(LPC_GPIO, PIN_PORT(pin), PIN_NO(pin));
    }
    else
    {
        Chip_GPIO_SetPinDIRInput(LPC_GPIO, PIN_PORT(pin), PIN_NO(pin));
    }
}

void mm_pin_write(uint32_t pin, uint8_t value)
{
    if (value == PIN_HIGH)
    {
        Chip_GPIO_SetPinOutHigh(LPC_GPIO, PIN_PORT(pin), PIN_NO(pin));
    }
    else
    {
        Chip_GPIO_SetPinOutLow(LPC_GPIO, PIN_PORT(pin), PIN_NO(pin));
    }
}

int8_t mm_pin_read(uint32_t pin)
{
    return Chip_GPIO_ReadPortBit(LPC_GPIO, PIN_PORT(pin), PIN_NO(pin));
}
