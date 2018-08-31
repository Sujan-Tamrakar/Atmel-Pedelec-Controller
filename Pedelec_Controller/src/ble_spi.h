/*
 * ble_spi.h
 *
 * Created: 30-Sep-14 8:09:16 AM
 *  Author: dploetzer
 */ 

#ifndef BLE_SPI_H_
#define BLE_SPI_H_

#define BLE_SPI                     (&AVR32_SPI1)
#define BLE_SPI_SCK_PIN             AVR32_PIN_PB21
#define BLE_SPI_SCK_FUNCTION        AVR32_SPI1_SCK_1_FUNCTION
#define BLE_SPI_MISO_PIN            AVR32_PIN_PB19
#define BLE_SPI_MISO_FUNCTION       AVR32_SPI1_MISO_1_FUNCTION
#define BLE_SPI_MOSI_PIN			AVR32_PIN_PB20
#define BLE_SPI_MOSI_FUNCTION       AVR32_SPI1_MOSI_1_FUNCTION

#define BLE_SPI_NPCS_PIN AVR32_SPI1_NPCS_3_2_PIN //CS3
#define BLE_SPI_NPCS_FLAGS (GPIO_INIT_HIGH | GPIO_DIR_OUTPUT)

#define BLE_SPI_RDY_PIN AVR32_PIN_PB23 //UH
#define BLE_SPI_RDY_FLAGS (GPIO_PULL_UP | GPIO_DIR_INPUT)

#define BLE_SPI_RST_PIN AVR32_PIN_PA25 //UL
#define BLE_SPI_RST_FLAGS (GPIO_INIT_HIGH | GPIO_DIR_OUTPUT)

//! SPI master speed in Hz.
#define BLE_SPI_MASTER_SPEED 400000

//! Number of bits in each SPI transfer.
#define BLE_SPI_BITS 8

#define BLE_SPI_NPCS 0

void ble_spi_init(void);
void ble_spi_reset(void);
spi_status_t ble_spi_selectChip(void);
spi_status_t ble_spi_unselectChip(void);
bool ble_spi_has_data(void);
uint8_t ble_spi_read(void);
spi_status_t ble_spi_write(uint8_t data);

#endif /* BLE_SPI_H_ */