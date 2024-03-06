/**
 * @file mm_sspi_def.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief  sspi 类型定义文件
 * 参考 mm_thread设备树
 * @version 0.1
 * @date 2024-03-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __MM_SSPI_DEF_H_
#define __MM_SSPI_DEF_H_

#include <stdint.h>
#ifdef __cplusplus
extern "C"
{
#endif

#define MM_SPI_CPHA (1 << 0) /* bit[0]:CPHA, clock phase */
#define MM_SPI_CPOL (1 << 1) /* bit[1]:CPOL, clock polarity */

#define MM_SPI_LSB (0 << 2) /* bit[2]: 0-LSB */
#define MM_SPI_MSB (1 << 2) /* bit[2]: 1-MSB */

#define MM_SPI_MASTER (0 << 3) /* SPI master device */
#define MM_SPI_SLAVE (1 << 3)  /* SPI slave device */

#define MM_SPI_CS_HIGH (1 << 4) /* Chipselect active high */
#define MM_SPI_NO_CS (1 << 5)   /* No chipselect */
#define MM_SPI_3WIRE (1 << 6)   /* SI/SO pin shared */
#define MM_SPI_READY (1 << 7)   /* Slave pulls low to pause */

#define MM_SPI_MODE_MASK (MM_SPI_CPHA | MM_SPI_CPOL | MM_SPI_MSB | MM_SPI_SLAVE | MM_SPI_CS_HIGH | MM_SPI_NO_CS | MM_SPI_3WIRE | MM_SPI_READY)

#define MM_SPI_MODE_0 (0 | 0)                     /* CPOL = 0, CPHA = 0 */
#define MM_SPI_MODE_1 (0 | MM_SPI_CPHA)           /* CPOL = 0, CPHA = 1 */
#define MM_SPI_MODE_2 (MM_SPI_CPOL | 0)           /* CPOL = 1, CPHA = 0 */
#define MM_SPI_MODE_3 (MM_SPI_CPOL | MM_SPI_CPHA) /* CPOL = 1, CPHA = 1 */

#define MM_SPI_BUS_MODE_SPI (1 << 0)
#define MM_SPI_BUS_MODE_QSPI (1 << 1)

    struct mm_driver
    {
        char name[8];
        uint8_t type;
        struct mm_driver *next;
        uint32_t ref_count;
    };
    /**
     * SPI configuration structure
     */
    struct mm_spi_configuration
    {
        uint8_t mode;
        uint8_t data_width;
        uint16_t reserved;

        uint32_t max_hz;
    };
    /**
     * SPI message structure
     */
    struct mm_spi_message
    {
        const void *send_buf;
        void *recv_buf;
        uint32_t length;
        struct mm_spi_message *next;

        unsigned cs_take : 1;
        unsigned cs_release : 1;
    };

    struct mm_sspi_bus
    {
        struct mm_driver parent;
        uint8_t mode;
        const struct mm_sspi_ops *ops;
        struct mm_sspi_device *owner;
    }; /**
        * SPI operators
        */
    struct mm_sspi_ops
    {
        int32_t (*configure)(struct mm_sspi_device *device, struct mm_spi_configuration *configuration);
        uint32_t (*xfer)(struct mm_sspi_device *device, struct mm_spi_message *message);
    };
    /**
     * SPI Virtual BUS, one device must connected to a virtual BUS
     */
    struct mm_sspi_device
    {
        struct mm_driver parent;
        struct mm_sspi_bus *bus;

        struct mm_spi_configuration config;
        uint32_t cs_pin;
        void *user_data;
    };
#ifdef __cplusplus
}
#endif
#endif
