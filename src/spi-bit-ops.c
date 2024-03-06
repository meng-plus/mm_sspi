/*
 * Copyright (c) 2006-2023, MM-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2021-10-11     kyle          first version
 */

#include "spi-bit-ops.h"
#include <string.h>
#include <assert.h>
#include "mm_pin.h"
#define TOG_SCLK(obj) obj->ops->tog_sclk(obj->data)
#define SET_SCLK(obj, val) obj->ops->set_sclk(obj->data, val)
#define SET_MOSI(obj, val) obj->ops->set_mosi(obj->data, val)
#define SET_MISO(obj, val) obj->ops->set_miso(obj->data, val)
#define GET_SCLK(obj) obj->ops->get_sclk(obj->data)
#define GET_MOSI(obj) obj->ops->get_mosi(obj->data)
#define GET_MISO(obj) obj->ops->get_miso(obj->data)
#define DIR_MOSI(obj, val) obj->ops->dir_mosi(obj->data, val)
#define DIR_MISO(obj, val) obj->ops->dir_miso(obj->data, val)

inline void spi_delay(struct mm_sspi_bit_obj *obj)
{
    obj->ops->udelay((obj->delay_us + 1) >> 1);
}

inline void spi_delay2(struct mm_sspi_bit_obj *obj)
{
    obj->ops->udelay(obj->delay_us);
}

#define SCLK_H(obj) SET_SCLK(obj, 1)
#define SCLK_L(obj) SET_SCLK(obj, 0)
#define MOSI_H(obj) SET_MOSI(obj, 1)
#define MOSI_L(obj) SET_MOSI(obj, 0)
#define MOSI_IN(obj) DIR_MOSI(obj, 1)
#define MOSI_OUT(obj) DIR_MOSI(obj, 0)
#define MISO_IN(obj) DIR_MISO(obj, 1)
#define MISO_OUT(obj) DIR_MISO(obj, 0)

inline int32_t spi_xfer_4line_data8(struct mm_sspi_bit_obj *obj,
                                    struct mm_spi_configuration *config,
                                    const void *send_buf,
                                    void *recv_buf,
                                    uint32_t length)
{
    int i = 0;

    assert(obj != NULL);
    assert(length != 0);

    {
        const uint8_t *send_ptr = send_buf;
        uint8_t *recv_ptr = recv_buf;
        uint32_t size = length;

        while (size--)
        {
            uint8_t tx_data = 0xFF;
            uint8_t rx_data = 0xFF;
            uint8_t bit = 0;

            if (send_buf != NULL)
            {
                tx_data = *send_ptr++;
            }

            for (i = 0; i < 8; i++)
            {
                if (config->mode & MM_SPI_MSB)
                {
                    bit = tx_data & (0x1 << (7 - i));
                }
                else
                {
                    bit = tx_data & (0x1 << i);
                }

                if (bit)
                    MOSI_H(obj);
                else
                    MOSI_L(obj);

                spi_delay2(obj);

                TOG_SCLK(obj);

                if (config->mode & MM_SPI_MSB)
                {
                    rx_data <<= 1;
                    bit = 0x01;
                }
                else
                {
                    rx_data >>= 1;
                    bit = 0x80;
                }

                if (GET_MISO(obj))
                {
                    rx_data |= bit;
                }
                else
                {
                    rx_data &= ~bit;
                }

                spi_delay2(obj);

                if (!(config->mode & MM_SPI_CPHA) || (size != 0) || (i < 7))
                {
                    TOG_SCLK(obj);
                }
            }

            if (recv_buf != NULL)
            {
                *recv_ptr++ = rx_data;
            }
        }
    }

    return length;
}

inline int32_t spi_xfer_4line_data16(struct mm_sspi_bit_obj *obj,
                                     struct mm_spi_configuration *config,
                                     const void *send_buf,
                                     void *recv_buf,
                                     uint32_t length)
{
    int i = 0;

    assert(obj != NULL);
    assert(length != 0);

    {
        const uint16_t *send_ptr = send_buf;
        uint16_t *recv_ptr = recv_buf;
        uint32_t size = length;

        while (size--)
        {
            uint16_t tx_data = 0xFFFF;
            uint16_t rx_data = 0xFFFF;
            uint16_t bit = 0;

            if (send_buf != NULL)
            {
                tx_data = *send_ptr++;
            }

            for (i = 0; i < 16; i++)
            {
                if (config->mode & MM_SPI_MSB)
                {
                    bit = tx_data & (0x1 << (15 - i));
                }
                else
                {
                    bit = tx_data & (0x1 << i);
                }

                if (bit)
                    MOSI_H(obj);
                else
                    MOSI_L(obj);

                spi_delay2(obj);

                TOG_SCLK(obj);

                if (config->mode & MM_SPI_MSB)
                {
                    rx_data <<= 1;
                    bit = 0x0001;
                }
                else
                {
                    rx_data >>= 1;
                    bit = 0x8000;
                }

                if (GET_MISO(obj))
                {
                    rx_data |= bit;
                }
                else
                {
                    rx_data &= ~bit;
                }

                spi_delay2(obj);

                if (!(config->mode & MM_SPI_CPHA) || (size != 0) || (i < 15))
                {
                    TOG_SCLK(obj);
                }
            }

            if (recv_buf != NULL)
            {
                *recv_ptr++ = rx_data;
            }
        }
    }

    return length;
}

inline int32_t spi_xfer_3line_data8(struct mm_sspi_bit_obj *obj,
                                    struct mm_spi_configuration *config,
                                    const void *send_buf,
                                    void *recv_buf,
                                    uint32_t length)
{
    int i = 0;

    assert(obj != NULL);
    assert(length != 0);

    {
        const uint8_t *send_ptr = send_buf;
        uint8_t *recv_ptr = recv_buf;
        uint32_t size = length;
        uint8_t send_flg = 0;

        if ((send_buf != NULL) || (recv_buf == NULL))
        {
            MOSI_OUT(obj);
            send_flg = 1;
        }
        else
        {
            MOSI_IN(obj);
        }

        while (size--)
        {
            uint8_t tx_data = 0xFF;
            uint8_t rx_data = 0xFF;
            uint8_t bit = 0;

            if (send_buf != NULL)
            {
                tx_data = *send_ptr++;
            }

            if (send_flg)
            {
                for (i = 0; i < 8; i++)
                {
                    if (config->mode & MM_SPI_MSB)
                    {
                        bit = tx_data & (0x1 << (7 - i));
                    }
                    else
                    {
                        bit = tx_data & (0x1 << i);
                    }

                    if (bit)
                        MOSI_H(obj);
                    else
                        MOSI_L(obj);

                    spi_delay2(obj);

                    TOG_SCLK(obj);

                    spi_delay2(obj);

                    if (!(config->mode & MM_SPI_CPHA) || (size != 0) || (i < 7))
                    {
                        TOG_SCLK(obj);
                    }
                }

                rx_data = tx_data;
            }
            else
            {
                for (i = 0; i < 8; i++)
                {
                    spi_delay2(obj);

                    TOG_SCLK(obj);

                    if (config->mode & MM_SPI_MSB)
                    {
                        rx_data <<= 1;
                        bit = 0x01;
                    }
                    else
                    {
                        rx_data >>= 1;
                        bit = 0x80;
                    }

                    if (GET_MOSI(obj))
                    {
                        rx_data |= bit;
                    }
                    else
                    {
                        rx_data &= ~bit;
                    }

                    spi_delay2(obj);

                    if (!(config->mode & MM_SPI_CPHA) || (size != 0) || (i < 7))
                    {
                        TOG_SCLK(obj);
                    }
                }
            }

            if (recv_buf != NULL)
            {
                *recv_ptr++ = rx_data;
            }
        }

        if (!send_flg)
        {
            MOSI_OUT(obj);
        }
    }

    return length;
}

inline int32_t spi_xfer_3line_data16(struct mm_sspi_bit_obj *obj,
                                     struct mm_spi_configuration *config,
                                     const void *send_buf,
                                     void *recv_buf,
                                     uint32_t length)
{
    int i = 0;

    assert(obj != NULL);
    assert(length != 0);

    {
        const uint16_t *send_ptr = send_buf;
        uint16_t *recv_ptr = recv_buf;
        uint32_t size = length;
        uint8_t send_flg = 0;

        if ((send_buf != NULL) || (recv_buf == NULL))
        {
            MOSI_OUT(obj);
            send_flg = 1;
        }
        else
        {
            MOSI_IN(obj);
        }

        while (size--)
        {
            uint16_t tx_data = 0xFFFF;
            uint16_t rx_data = 0xFFFF;
            uint16_t bit = 0;

            if (send_buf != NULL)
            {
                tx_data = *send_ptr++;
            }

            if (send_flg)
            {
                for (i = 0; i < 16; i++)
                {
                    if (config->mode & MM_SPI_MSB)
                    {
                        bit = tx_data & (0x1 << (15 - i));
                    }
                    else
                    {
                        bit = tx_data & (0x1 << i);
                    }

                    if (bit)
                        MOSI_H(obj);
                    else
                        MOSI_L(obj);

                    spi_delay2(obj);

                    TOG_SCLK(obj);

                    spi_delay2(obj);

                    if (!(config->mode & MM_SPI_CPHA) || (size != 0) || (i < 15))
                    {
                        TOG_SCLK(obj);
                    }
                }

                rx_data = tx_data;
            }
            else
            {
                for (i = 0; i < 16; i++)
                {
                    spi_delay2(obj);

                    TOG_SCLK(obj);

                    if (config->mode & MM_SPI_MSB)
                    {
                        rx_data <<= 1;
                        bit = 0x0001;
                    }
                    else
                    {
                        rx_data >>= 1;
                        bit = 0x8000;
                    }

                    if (GET_MOSI(obj))
                    {
                        rx_data |= bit;
                    }
                    else
                    {
                        rx_data &= ~bit;
                    }

                    spi_delay2(obj);

                    if (!(config->mode & MM_SPI_CPHA) || (size != 0) || (i < 15))
                    {
                        TOG_SCLK(obj);
                    }
                }
            }

            if (recv_buf != NULL)
            {
                *recv_ptr++ = rx_data;
            }
        }

        if (!send_flg)
        {
            MOSI_OUT(obj);
        }
    }

    return length;
}

int32_t spi_bit_configure(struct mm_sspi_device *device, struct mm_spi_configuration *configuration)
{
    struct mm_sspi_bit_obj *obj = (struct mm_sspi_bit_obj *)device->bus;

    assert(device != 0);
    assert(configuration != 0);

    if (configuration->mode & MM_SPI_SLAVE)
    {
        return -1;
    }

    if (configuration->mode & MM_SPI_CPOL)
    {
        SCLK_H(obj);
    }
    else
    {
        SCLK_L(obj);
    }

    if (configuration->max_hz < 200000)
    {
        obj->delay_us = 1;
    }
    else
    {
        obj->delay_us = 0;
    }

    memcpy(&obj->config, configuration, sizeof(struct mm_spi_configuration));

    return 0;
}

uint32_t spi_bit_xfer(struct mm_sspi_device *device, struct mm_spi_message *message)
{
    struct mm_sspi_bit_obj *obj = (struct mm_sspi_bit_obj *)device->bus;
    struct mm_spi_configuration *config = &obj->config;
    uint32_t cs_pin = device->cs_pin;

    assert(device != NULL);
    assert(message != NULL);

#ifdef MM_SPI_BITOPS_DEBUG
    if (!ops->tog_sclk || !ops->set_sclk || !ops->get_sclk)
    {
        // LOG_E("SPI bus error, SCLK line not defined");
    }
    if (!ops->set_mosi || !ops->get_mosi)
    {
        // LOG_E("SPI bus error, MOSI line not defined");
    }
    if (!ops->set_miso || !ops->get_miso)
    {
        // LOG_E("SPI bus error, MISO line not defined");
    }
#endif

    /* take CS */
    if (message->cs_take && (cs_pin != PIN_NONE))
    {
        // LOG_I("spi take cs\n");
        mm_pin_write(cs_pin, PIN_LOW);
        spi_delay(obj);

        /* spi phase */
        if (config->mode & MM_SPI_CPHA)
        {
            spi_delay(obj);
            TOG_SCLK(obj);
        }
    }

    if (config->mode & MM_SPI_3WIRE)
    {
        if (config->data_width <= 8)
        {
            spi_xfer_3line_data8(obj,
                                 config,
                                 message->send_buf,
                                 message->recv_buf,
                                 message->length);
        }
        else if (config->data_width <= 16)
        {
            spi_xfer_3line_data16(obj,
                                  config,
                                  message->send_buf,
                                  message->recv_buf,
                                  message->length);
        }
    }
    else
    {
        if (config->data_width <= 8)
        {
            spi_xfer_4line_data8(obj,
                                 config,
                                 message->send_buf,
                                 message->recv_buf,
                                 message->length);
        }
        else if (config->data_width <= 16)
        {
            spi_xfer_4line_data16(obj,
                                  config,
                                  message->send_buf,
                                  message->recv_buf,
                                  message->length);
        }
    }

    /* release CS */
    if (message->cs_release && (cs_pin != 0))
    {
        spi_delay(obj);
        mm_pin_write(cs_pin, PIN_HIGH);
        // LOG_I("spi release cs\n");
    }

    return message->length;
}

static struct mm_sspi_ops spi_bit_bus_ops =
    {
        .configure = spi_bit_configure,
        .xfer = spi_bit_xfer,
};

/**
 * @brief 添加 SSPI 位总线
 *
 * 将给定的 SSPI 位对象添加到指定的总线名称，并设置相应的操作函数。
 *
 * @param obj SSPI 位对象指针
 * @param bus_name 总线名称
 * @param ops SSPI 位操作函数指针
 *
 * @return 成功返回 0，失败返回错误码
 */
int32_t mm_sspi_bit_add_bus(struct mm_sspi_bit_obj *obj,
                            const char *bus_name,
                            const struct mm_sspi_bit_ops *ops)
{
    // 将传入的ops赋值给obj的ops成员变量
    obj->ops = ops;

    // 设置obj的config数据宽度为8位
    obj->config.data_width = 8;

    // 设置obj的config最大频率为1MHz
    obj->config.max_hz = 1 * 1000 * 1000;

    // 设置obj的config模式为：主模式、最高位在前、模式0
    obj->config.mode = MM_SPI_MASTER | MM_SPI_MSB | MM_SPI_MODE_0;

    // 根据配置模式设置空闲状态
    /* idle status */
    if (obj->config.mode & MM_SPI_CPOL)
        // 若模式包含CPOL，则设置时钟为高电平
        SCLK_H(obj);
    else
        // 否则设置时钟为低电平
        SCLK_L(obj);

    // 注册bus，并返回结果
    return mm_sspi_bus_register(&obj->bus, bus_name, &spi_bit_bus_ops);
}
