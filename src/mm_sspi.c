/**
 * @file mm_sspi.c
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 源码
 * @version 0.1
 * @date 2024-03-05
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "mm_sspi.h"
#include "mm_pin.h"
#include <string.h>
#include "stdio.h"
#include <assert.h>
static struct mm_driver *driver_list = NULL;
int mm_sspi_bus_register(struct mm_sspi_bus *bus, const char *name, const struct mm_sspi_ops *ops)
{
    // 检查参数是否为空
    if (bus == NULL || name == NULL || ops == NULL)
    {
        return -1; // 返回错误代码表示参数无效
    }

    // 将ops赋值给bus的ops成员
    bus->ops = ops;

    // 使用snprintf确保字符串以null字符终止
    snprintf(bus->parent.name, sizeof(bus->parent.name), "%s", name);

    // 硬编码将bus的mode成员设置为MM_SPI_BUS_MODE_SPI
    // 硬编码，但暂时保持不变
    bus->mode = MM_SPI_BUS_MODE_SPI; // 硬编码，但暂时保持不变

    // 将bus的owner成员设置为NULL
    bus->owner = NULL;

    // 将bus添加到driver_list链表的头部
    bus->parent.next = driver_list;
    driver_list = &bus->parent;

    // 返回0表示注册成功
    return 0;
}
int32_t mm_sspi_bus_attach_device_cspin(struct mm_sspi_device *device, const char *name, const char *bus_name, uint32_t cs_pin)
{

    struct mm_sspi_bus *bus = (struct mm_sspi_bus *)mm_device_find(bus_name);
    if (bus == 0)
    {
        return -1;
    }

    // 使用snprintf确保字符串以null字符终止
    snprintf(device->parent.name, sizeof(device->parent.name), "%s", name);

    device->bus = bus;
    device->cs_pin = cs_pin;
    if (cs_pin != -1)
    {

        // often active low, output from master
        mm_pin_mode(cs_pin, PIN_MODE_OUTPUT);
        mm_pin_write(cs_pin, PIN_HIGH);
    }
    // 将bus添加到driver_list链表的头部
    device->parent.next = driver_list;
    driver_list = &device->parent;

    return 0;
}

struct mm_driver *mm_device_find(const char *name)
{
    struct mm_driver *item = driver_list;
    while (item)
    {
        if (strncmp(name, item->name, sizeof(item->name)) == 0)
        {
            return item;
        }
        item = item->next;
    }

    return NULL;
}

int32_t mm_sspi_transfer(struct mm_sspi_device *device, const void *send_buf, void *recv_buf, uint32_t length)
{
    int32_t result;
    struct mm_spi_message message;

    assert(device != NULL);
    assert(device->bus != NULL);

    if (device->bus->owner != device)
    {
        /* not the same owner as current, re-configure SPI bus */
        result = device->bus->ops->configure(device, &device->config);
        if (result == 0)
        {
            /* set SPI bus owner */
            device->bus->owner = device;
        }
        else
        {
            /* configure SPI bus failed */
            // LOG_E("SPI device %s configuration failed", device->parent.parent.name);
            goto __exit;
        }
    }

    /* initial message */
    message.send_buf = send_buf;
    message.recv_buf = recv_buf;
    message.length = length;
    message.cs_take = 1;
    message.cs_release = 1;
    message.next = NULL;

    /* transfer message */
    result = device->bus->ops->xfer(device, &message);
__exit:
    return result;
}
