/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2021-10-11     kyle          first version
 * 2022-6-14      solar         Remove the const attribute of private data in ops
 */

#ifndef __SPI_BIT_OPS_H__
#define __SPI_BIT_OPS_H__

#include "mm_sspi.h"

#ifdef __cplusplus
extern "C"
{
#endif

    struct mm_sspi_bit_ops
    {

        void (*const tog_sclk)(void *data);
        void (*const set_sclk)(void *data, int32_t state);
        void (*const set_mosi)(void *data, int32_t state);
        void (*const set_miso)(void *data, int32_t state);
        int32_t (*const get_sclk)(void *data);
        int32_t (*const get_mosi)(void *data);
        int32_t (*const get_miso)(void *data);

        void (*const dir_mosi)(void *data, int32_t state);
        void (*const dir_miso)(void *data, int32_t state);

        void (*const udelay)(uint32_t us);
    };

    struct mm_sspi_bit_obj
    {
        struct mm_sspi_bus bus;
        const struct mm_sspi_bit_ops *ops;
        struct mm_spi_configuration config;
        uint32_t delay_us; /* sclk, mosi and miso line delay */
        void *data;        /* private data for lowlevel routines */
    };

    int32_t mm_sspi_bit_add_bus(struct mm_sspi_bit_obj *obj,
                                const char *bus_name,
                                const struct mm_sspi_bit_ops *ops);

#ifdef __cplusplus
}
#endif

#endif
