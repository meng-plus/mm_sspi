/**
 * @file lpc_bare_sspi_port.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief sspi组件的lpc接口文件
 * @version 0.1
 * @date 2024-03-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __LPC_BARE_SSPI_H_
#define __LPC_BARE_SSPI_H_

#include "mm_sspi.h"
#include "spi-bit-ops.h"
#ifdef __cplusplus
extern "C"
{
#endif

    /* lpc soft spi config */
    struct lpc_soft_spi_config
    {
        uint8_t sck; //=PIN_NUM(port, no)
        uint8_t mosi;
        uint8_t miso;
        const char *bus_name;
    };

    /* lpc soft spi dirver */
    struct lpc_soft_spi
    {
        struct mm_sspi_bit_obj spi;
    };

    void lpc_bare_sspi_init();

#ifdef __cplusplus
}
#endif
#endif
