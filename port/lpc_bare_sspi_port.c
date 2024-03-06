#include "lpc_bare_sspi_port.h"
#include "mm_pin.h"
#include "Chip.h"
#ifndef ITEM_NUM
#define ITEM_NUM(items) sizeof(items) / sizeof(items[0])
#endif
// static struct spi_bus_device *spi_bus_device_head = NULL;

static void lpc_sspi_gpio_init(struct lpc_soft_spi *spi)
{
    struct lpc_soft_spi_config *cfg = (struct lpc_soft_spi_config *)spi->spi.data;
    mm_pin_mode(cfg->sck, PIN_MODE_OUTPUT);
    mm_pin_mode(cfg->miso, PIN_MODE_INPUT);
    mm_pin_mode(cfg->mosi, PIN_MODE_OUTPUT);

    mm_pin_write(cfg->miso, PIN_HIGH);
    mm_pin_write(cfg->sck, PIN_HIGH);
    mm_pin_write(cfg->mosi, PIN_HIGH);
}

void lpc_tog_sclk(void *data)
{
    struct lpc_soft_spi_config *cfg = (struct lpc_soft_spi_config *)data;
    if (mm_pin_read(cfg->sck) == PIN_HIGH)
    {
        mm_pin_write(cfg->sck, PIN_LOW);
    }
    else
    {
        mm_pin_write(cfg->sck, PIN_HIGH);
    }
}

void lpc_set_sclk(void *data, int32_t state)
{

    struct lpc_soft_spi_config *cfg = (struct lpc_soft_spi_config *)data;
    if (state)
    {
        mm_pin_write(cfg->sck, PIN_HIGH);
    }
    else
    {
        mm_pin_write(cfg->sck, PIN_LOW);
    }
}

void lpc_set_mosi(void *data, int32_t state)
{
    struct lpc_soft_spi_config *cfg = (struct lpc_soft_spi_config *)data;
    if (state)
    {
        mm_pin_write(cfg->mosi, PIN_HIGH);
    }
    else
    {
        mm_pin_write(cfg->mosi, PIN_LOW);
    }
}

void lpc_set_miso(void *data, int32_t state)
{
    struct lpc_soft_spi_config *cfg = (struct lpc_soft_spi_config *)data;
    if (state)
    {
        mm_pin_write(cfg->miso, PIN_HIGH);
    }
    else
    {
        mm_pin_write(cfg->miso, PIN_LOW);
    }
}

int32_t lpc_get_sclk(void *data)
{
    struct lpc_soft_spi_config *cfg = (struct lpc_soft_spi_config *)data;
    return mm_pin_read(cfg->sck);
}

int32_t lpc_get_mosi(void *data)
{
    struct lpc_soft_spi_config *cfg = (struct lpc_soft_spi_config *)data;
    return mm_pin_read(cfg->mosi);
}

int32_t lpc_get_miso(void *data)
{
    struct lpc_soft_spi_config *cfg = (struct lpc_soft_spi_config *)data;
    return mm_pin_read(cfg->miso);
}

void lpc_dir_mosi(void *data, int32_t state)
{
    struct lpc_soft_spi_config *cfg = (struct lpc_soft_spi_config *)data;
    if (state)
    {
        mm_pin_mode(cfg->mosi, PIN_MODE_INPUT);
    }
    else
    {
        mm_pin_mode(cfg->mosi, PIN_MODE_OUTPUT);
    }
}

void lpc_dir_miso(void *data, int32_t state)
{
    struct lpc_soft_spi_config *cfg = (struct lpc_soft_spi_config *)data;
    if (state)
    {
        mm_pin_mode(cfg->miso, PIN_MODE_INPUT);
    }
    else
    {
        mm_pin_mode(cfg->miso, PIN_MODE_OUTPUT);
    }
}

static void lpc_udelay(uint32_t us)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;

    ticks = us * reload / (1000000UL / 1000);
    told = SysTick->VAL;
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;
            }
        }
    }
}

static const struct mm_sspi_bit_ops sspi_bit_ops =
    {
        .tog_sclk = lpc_tog_sclk,
        .set_sclk = lpc_set_sclk,
        .set_mosi = lpc_set_mosi,
        .set_miso = lpc_set_miso,
        .get_sclk = lpc_get_sclk,
        .get_mosi = lpc_get_mosi,
        .get_miso = lpc_get_miso,
        .dir_mosi = lpc_dir_mosi,
        .dir_miso = lpc_dir_miso,
        .udelay = lpc_udelay,

};
#define LD_BLU1 PIN_NUM(2, 13)
#define LD_BLU3 PIN_NUM(1, 27)
#define LD_BLU5 PIN_NUM(1, 29)
#define LD_DEN PIN_NUM(2, 4)
#define LD_GRN5 PIN_NUM(1, 25)
void lpc_bare_sspi_init()
{
    static const struct lpc_soft_spi_config soft_spi_config[] = {
        {.sck = PIN_NUM(2, 4),   // LD_DEN
         .mosi = PIN_NUM(1, 27), // LD_BLU3
         .miso = PIN_NUM(1, 25), // LD_GRN5
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
#define RT_SFUD_DEFAULT_SPI_CFG
}
