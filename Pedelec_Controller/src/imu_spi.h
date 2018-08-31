/*
 * imu_spi.h
 *
 * Created: 21-Feb-17 8:09:16 AM
 *  Author: STamrakar
 */ 

#ifndef IMU_SPI_H_
#define IMU_SPI_H_

#define IMU_SPI                     (&AVR32_SPI0)
#define IMU_SPI_SCK_PIN             AVR32_PIN_PD02
#define IMU_SPI_SCK_FUNCTION        AVR32_SPI0_SCK_FUNCTION
#define IMU_SPI_MISO_PIN            AVR32_PIN_PD01
#define IMU_SPI_MISO_FUNCTION       AVR32_SPI0_MISO_FUNCTION
#define IMU_SPI_MOSI_PIN			AVR32_PIN_PD00
#define IMU_SPI_MOSI_FUNCTION       AVR32_SPI0_MOSI_FUNCTION
#define IMU_SPI_NPCS_PIN            AVR32_SPI0_NPCS_0_PIN
#define IMU_SPI_NPCS_FUNCTION       AVR32_SPI0_NPCS_0_FUNCTION
#define SPI_SLAVECHIP_NUMBER		(0)

#define IMU_SPI_NPCS_PIN AVR32_SPI0_NPCS_0_PIN //CS0
#define IMU_SPI_NPCS_FLAGS (GPIO_INIT_HIGH | GPIO_DIR_OUTPUT)

#define IMU_SPI_RDY_PIN AVR32_PIN_PB04 //UH
#define IMU_SPI_RDY_FLAGS (GPIO_PULL_UP | GPIO_DIR_INPUT)

#define IMU_SPI_RST_PIN AVR32_PIN_PB05 //UL
#define IMU_SPI_RST_FLAGS (GPIO_INIT_HIGH | GPIO_DIR_OUTPUT)

//! SPI master speed in Hz.
#define IMU_SPI_MASTER_SPEED 1000000

//! Number of bits in each SPI transfer.
#define IMU_SPI_BITS 8

#define IMU_SPI_NPCS 0

void spi_init_module(void);
void conf_duplex(uint16_t *confdata, size_t confsize);
void imu_spi_configuration();

#endif /* IMU_SPI_H_ */