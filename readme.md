---
title: sspi开源库
keywords: sspi,sspi开源库,软件SPI,单片机,嵌入式,开源
description: 软件SPI(Serial Peripheral Interface)在单片机开发中经常被使用，通过模块化的设计简化设计避免重复的工作量。
author: 蒙蒙plus
timestamp: 2024-03-18
version: 1.0.0
---
# sspi开源库

## 简介
软件SPI(Serial Peripheral Interface)在单片机开发中经常被使用，通过模块化的设计简化设计避免重复的工作量，

## 名词介绍

| 简称   | 全称                               | 中文描述    |
| ---- | -------------------------------- | ------- |
| SSPI | soft Serial Peripheral Interface | 软串行外围接口 |
|      |                                  |         |

## 目录介绍
```bash
.
├── inc
│   ├── mm_pin.h                #引脚操作头文件
│   ├── mm_sspi_def.h
│   ├── mm_sspi.h
│   └── spi-bit-ops.h
├── port                        #接口移植文件
│   ├── lpc_bare_sspi_port.c
│   ├── lpc_bare_sspi_port.h
│   └── mm_pin_port.c           #引脚操作实现文件
├── readme.md                   #文档
└── src
    ├── mm_sspi.c
    └── spi-bit-ops.c
```
## 快速开始
 1. 添加sspi源码到工程中
 2. 添加sspi头文件路径到工程中
 3. 添加引脚操作实现文件到工程中
 4. 添加引脚操作头文件到工程中
## 示例
```c
#define LD_BLU1 PIN_NUM(2, 13)
#define LD_BLU3 PIN_NUM(1, 27)
#define LD_BLU5 PIN_NUM(1, 29)
#define LD_DEN PIN_NUM(2, 4)
#define LD_GRN5 PIN_NUM(1, 25)
void lpc_bare_sspi_init()
{
    static const struct lpc_soft_spi_config soft_spi_config[] = {
        {.sck = PIN_NUM(2, 4),   // LD_DEN
         .mosi = PIN_NUM(1, 25), // LD_GRN5
         .miso = PIN_NUM(1, 27), // LD_BLU3
         .bus_name = "sspi1"}};

    static struct lpc_soft_spi mm_sspi_bus[ITEM_NUM(soft_spi_config)];
    static uint8_t init_ok = 0;
    if (init_ok)
    {
        return;
    }
    for (int i = 0; i < ITEM_NUM(soft_spi_config); i++)
    {
        mm_sspi_bus[i].spi.data = (void *)&soft_spi_config[i];
        lpc_sspi_gpio_init(&mm_sspi_bus[i]);
        mm_sspi_bit_add_bus(&mm_sspi_bus[i].spi, soft_spi_config[i].bus_name, &sspi_bit_ops);
    }
    static struct mm_sspi_device sspi10 = {
        .config = {
            .mode = MM_SPI_MODE_3 | MM_SPI_MSB,
            .data_width = 8,
            .max_hz = 100000,
        },
    };
    mm_sspi_bus_attach_device_cspin(&sspi10,
                                    "sspi10",
                                    "sspi1",
                                    PIN_NUM(1, 29)); // LD_BLU5
}
```
## 贡献
欢迎提交issue和pr.

## 联系我
您可以通过qq群(790012859)联系到作者
## 许可证
[Apache License 2.0](https://github.com/mmonplus/sspi/blob/master/LICENSE)
## 鸣谢
[1] [https://github.com/rt-thread/rt-thread](https://github.com/rt-thread/rt-thread)

