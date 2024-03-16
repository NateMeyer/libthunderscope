/* SPDX-License-Identifier: BSD-2-Clause
 *
 * Thunderscope library
 *
 * Copyright (C) 2024 / Nate Meyer  / nate.devel@gmail.com
 *
 */
#ifndef _SPI_H_
#define _SPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#include "gpio.h"
#include "liblitepcie.h"


#define SPI_CTRL_START      (1 << 0)
#define SPI_CTRL_LENGTH     (1 << 8)
#define SPI_STATUS_DONE     (1 << 0)
#define SPI_TIMEOUT_US      100000 /* in us */

typedef struct spi_bus_s
{
    file_t fd;
    gpio_t cs_set;
}spi_bus_t;

typedef struct spi_dev_s
{
    spi_bus_t* bus;
    gpio_t cs;
}spi_dev_t;

int32_t spi_bus_init(spi_bus_t* bus, file_t fd, gpio_t cs_mask);
int32_t spi_dev_init(spi_dev_t* dev, spi_bus_t* bus, gpio_t cs);

void spi_write(spi_dev_t dev, uint8_t reg, uint8_t* data, uint8_t len);
bool spi_is_busy(spi_dev_t dev);
int32_t spi_busy_wait(spi_dev_t dev);

#ifdef __cplusplus
}
#endif
#endif