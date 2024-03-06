/**
 * @file mm_pin.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief  参考 rt-thread
 * @version 0.1
 * @date 2024-03-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __MM_PIN_H_
#define __MM_PIN_H_

#include <stdint.h>
#ifdef __cplusplus
extern "C"
{
#endif

#define PIN_NUM(port, no) (((((port) & 0x1Fu) << 5) | ((no) & 0x1Fu)))
#define PIN_PORT(pin) ((uint8_t)(((pin) >> 5) & 0x1Fu))
#define PIN_NO(pin) ((uint8_t)((pin) & 0x1Fu))

#define PIN_MODE_OUTPUT 1
#define PIN_MODE_INPUT 0
#define PIN_HIGH 1
#define PIN_LOW 0
#define PIN_NONE -1
    void mm_pin_mode(uint32_t pin, uint8_t mode);
    void mm_pin_write(uint32_t pin, uint8_t value);
    int8_t mm_pin_read(uint32_t pin);
#ifdef __cplusplus
}
#endif
#endif
