/*
 * ble_spi.c
 *
 * Created: 30-Sep-14 8:09:01 AM
 *  Author: dploetzer
 */ 

#include <asf.h>
#include "ble_spi.h"

uint8_t reverse_byte(uint8_t v)
{
	unsigned int s = 8;
	unsigned int mask = ~0;
	while ((s >>= 1) > 0)
	{
		mask ^= (mask << s);
		v = ((v >> s) & mask) | ((v << s) & ~mask);
	}
	return v;
}

void ble_spi_init()
{
	
	static const gpio_map_t BLE_SPI_GPIO_MAP =
	{
		{BLE_SPI_MISO_PIN, BLE_SPI_MISO_FUNCTION},  // MISO
		{BLE_SPI_MOSI_PIN, BLE_SPI_MOSI_FUNCTION},  // MOSI
		{BLE_SPI_SCK_PIN, BLE_SPI_SCK_FUNCTION},  // SPI Clock
		// Chip Select NPCS handled manually!
	};
	
	// SPI options
	spi_options_t spiOptions =
	{
		.reg          = BLE_SPI_NPCS,
		.baudrate     = BLE_SPI_MASTER_SPEED,
		.bits         = BLE_SPI_BITS,
		.spck_delay   = 0,
		.trans_delay  = 0,
		.stay_act     = 1,
		.spi_mode     = 0,
		.modfdis      = 1
	};

	// Assign I/Os to SPI
	gpio_enable_module(BLE_SPI_GPIO_MAP, sizeof(BLE_SPI_GPIO_MAP) / sizeof(BLE_SPI_GPIO_MAP[0]));

	// Initialize as master
	spi_initMaster(BLE_SPI, &spiOptions);

	// Set SPI selection mode: variable_ps, pcs_decode, delay
	spi_selectionMode(BLE_SPI, 0, 0, 0);

	// Enable SPI module
	spi_enable(BLE_SPI);
	
	spi_setupChipReg(BLE_SPI, &spiOptions, FOSC0);

	//configuration of manually manipulated pins
	gpio_configure_pin(BLE_SPI_NPCS_PIN, BLE_SPI_NPCS_FLAGS);
	gpio_configure_pin(BLE_SPI_RST_PIN, BLE_SPI_RST_FLAGS);
	gpio_configure_pin(BLE_SPI_RDY_PIN, BLE_SPI_RDY_FLAGS);
	
	ble_spi_reset();
}

void ble_spi_reset()
{
	gpio_clr_gpio_pin(BLE_SPI_RST_PIN);
	delay_ms(50);
	gpio_set_gpio_pin(BLE_SPI_RST_PIN);
	delay_ms(50);
}

spi_status_t ble_spi_selectChip()
{
	gpio_clr_gpio_pin(BLE_SPI_NPCS_PIN);
	return spi_selectChip(BLE_SPI, BLE_SPI_NPCS);
}

spi_status_t ble_spi_unselectChip()
{
	gpio_set_gpio_pin(BLE_SPI_NPCS_PIN);
	return spi_unselectChip(BLE_SPI, BLE_SPI_NPCS);
}

bool ble_spi_has_data()
{
	return (!gpio_get_pin_value(BLE_SPI_RDY_PIN));
}

uint8_t ble_spi_read()
{
	uint16_t rxdata;
	spi_write(BLE_SPI,0x00);
	spi_read(BLE_SPI,&rxdata);
	return reverse_byte((uint8_t)rxdata);
}

spi_status_t ble_spi_write(uint8_t data)
{
	spi_status_t spi_status = spi_write(BLE_SPI,reverse_byte(data));
	while(!spi_is_tx_empty(BLE_SPI));
	return spi_status;
}