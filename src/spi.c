/* SPDX-License-Identifier: BSD-2-Clause
 *
 * Thunderscope library
 *
 * Copyright (C) 2024 / Nate Meyer  / nate.devel@gmail.com
 *
 */
#include "spi.h"
#include "liblitepcie.h"

#include <time.h>


static bool spi_check_timeout(const struct timespec * start, uint32_t timeout);


int32_t spi_bus_init(spi_bus_t* bus, file_t fd, gpio_t cs_mask)
{
    if(bus == NULL)
    {
        return -1;
    }

    bus->fd = fd;
    bus->cs_set = cs_mask;

    return 0;
}

int32_t spi_dev_init(spi_dev_t* dev, spi_bus_t* bus, gpio_t cs)
{
    if(bus == NULL || dev == NULL)
    {
        return -1;
    }

    dev->bus = bus;
    dev->cs = cs;

    return 0;
}

void spi_write(spi_dev_t dev, uint8_t reg, uint8_t* data, uint8_t len) {

    // Set Chip Select.
    //litepcie_writel(fd, CSR_MAIN_SPI_CS_ADDR, (1 << cs));
    gpio_clear(dev.bus->cs_set);

    // Prepare MOSI data.
    uint32_t mosi_data = (reg << 16) + (data[0] << 8) + data[1]; 

    litepcie_writel(dev.bus->fd, CSR_MAIN_SPI_MOSI_ADDR, mosi_data);

    // Start SPI Xfer.
    litepcie_writel(dev.bus->fd, CSR_MAIN_SPI_CONTROL_ADDR, 24 * SPI_CTRL_LENGTH | SPI_CTRL_START);
}

bool spi_is_busy(spi_dev_t dev)
{
    return litepcie_readl(dev.bus->fd, CSR_MAIN_SPI_STATUS_ADDR) != SPI_STATUS_DONE;
}

// Wait SPI Xfer to be done.
int32_t spi_busy_wait(spi_dev_t dev)
{
    int32_t busyError = -1;
    struct timespec timeStart;
    timespec_get(&timeStart, TIME_UTC);

    do {
        if (litepcie_readl(dev.bus->fd, CSR_MAIN_SPI_STATUS_ADDR) == SPI_STATUS_DONE)
        {
            break;
        }
    } while (!spi_check_timeout(&timeStart, SPI_TIMEOUT_US));
    
    return busyError;
}


static bool spi_check_timeout(const struct timespec * start, uint32_t timeout)
{
    struct timespec timeNow;
    timespec_get(&timeNow, TIME_UTC);

    uint32_t delta_us = ((timeNow.tv_sec - start->tv_sec) * 1000000)
                            + ((timeNow.tv_nsec - start->tv_nsec) / 1000);
    return (delta_us > timeout);
}