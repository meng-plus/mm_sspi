/**
 * @file mm_sspi.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief  sspi 接口文件
 * 参考 mm_thread设备树
 * @version 0.1
 * @date 2024-03-05
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef __MM_SSPI_H_
#define __MM_SSPI_H_

#include "mm_sspi_def.h"
#ifdef __cplusplus
extern "C"
{
#endif

    /* register a SPI bus */
    int mm_sspi_bus_register(struct mm_sspi_bus *bus, const char *name, const struct mm_sspi_ops *ops);
    /* attach a device on SPI bus */

    int32_t mm_sspi_bus_attach_device_cspin(struct mm_sspi_device *device,
                                            const char *name,
                                            const char *bus_name,
                                            uint32_t cs_pin);

    struct mm_driver *mm_device_find(const char *name);

    /**
     * This function transmits data to SPI device.
     *
     * @param device the SPI device attached to SPI bus
     * @param send_buf the buffer to be transmitted to SPI device.
     * @param recv_buf the buffer to save received data from SPI device.
     * @param length the length of transmitted data.
     *
     * @return the actual length of transmitted.
     */
    int32_t mm_sspi_transfer(struct mm_sspi_device *device,
                             const void *send_buf,
                             void *recv_buf,
                             uint32_t length);
    inline int32_t mm_sspi_recv(struct mm_sspi_device *device,
                                void *recv_buf,
                                uint32_t length)
    {
        return mm_sspi_transfer(device, 0, recv_buf, length);
    }

    inline int32_t mm_sspi_send(struct mm_sspi_device *device,
                                const void *send_buf,
                                uint32_t length)
    {
        return mm_sspi_transfer(device, send_buf, 0, length);
    }
#ifdef __cplusplus
}
#endif
#endif
