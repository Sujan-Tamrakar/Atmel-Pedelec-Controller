/*
 * imu_spi.c
 *
 * Created: 21-Feb-17 8:09:16 AM
 *  Author: STamrakar
 */

#include <asf.h>
#include "imu_spi.h"

void spi_init_module(void)
{
	spi_options_t my_spi_options={
		// The SPI channel to set up : Memory is connected to CS1
		SPI_SLAVECHIP_NUMBER,
		// Preferred baudrate for the SPI.
		1000,
		// Number of bits in each character (8 to 16).
		16,
		// Delay before first clock pulse after selecting slave (in PBA clock periods).
		0,
		// Delay between each transfer/character (in PBA clock periods).
		0,
		// Sets this chip to stay active after last transfer to it.
		1,
		// Which SPI mode to use when transmitting.
		SPI_MODE_0,
		// Disables the mode fault detection.
		// With this bit cleared, the SPI master mode will disable itself if another
		// master tries to address it.
		1
	};
	
	//Init SPI module as master
	spi_initMaster(IMU_SPI,&my_spi_options);
	//Setup configuration for chip connected to CS1
	spi_setupChipReg(IMU_SPI,&my_spi_options,sysclk_get_pba_hz());
	//Allow the module to transfer data
	spi_enable(IMU_SPI);
	
	static const gpio_map_t IMU_SPI_GPIO_MAP =
	{
		{IMU_SPI_MISO_PIN, IMU_SPI_MISO_FUNCTION},  // MISO
		{IMU_SPI_MOSI_PIN, IMU_SPI_MOSI_FUNCTION},  // MOSI
		{IMU_SPI_SCK_PIN, IMU_SPI_SCK_FUNCTION},  // SPI Clock
		{IMU_SPI_NPCS_PIN,IMU_SPI_NPCS_FUNCTION}  // CS
		// Chip Select NPCS handled manually!
	};
	
	// Assign I/Os to SPI
	gpio_enable_module(IMU_SPI_GPIO_MAP, sizeof(IMU_SPI_GPIO_MAP) / sizeof(IMU_SPI_GPIO_MAP[0]));
}

void duplex_com(uint16_t *confdata, size_t confsize){
	int i = 0;
	for(i = 0 ; i < confsize; i++){	
		//Select given device on the SPI bus
		spi_selectChip(IMU_SPI, SPI_SLAVECHIP_NUMBER);
		
		//put the address(based on confdata
		spi_put(IMU_SPI,confdata[i]);
		
		//Wait for a complete transmission
		while(!spi_is_tx_empty(IMU_SPI));
		
		// De-select the slave
		spi_unselectChip(IMU_SPI,SPI_SLAVECHIP_NUMBER);
	}
}

void imu_spi_configuration(){
	sysclk_init();
	spi_init_module();
	
	//Wait for the transmitter to be ready
	while(!spi_is_tx_ready(IMU_SPI));	
	
	//specify the confdata (address)
	uint16_t confdata[] = {0xF500, // Read Who am I,
						   0x6B00, // Write PWR_MGMT_1 Registers
						   0x6C07, // Write PWR_MGMT_2 Registers
						   0x1D09, // Write ACCEL_CONFIG2 Registers
						   0x38E0, // Write INT_ENABLE Registers
						   0x2000, // Write ACCEL_WOM_X_THR Registers
						   0x2100, // Write ACCEL_WOM_Y_THR Registers
						   0x2200, // Write ACCEL_WOM_Z_THR Registers
						   0x69C1, // Write ACCEL_INTEL_CTRL Registers
						   0x1900 // Write SMPLRT_DIV Registers
						  };
	duplex_com(confdata,sizeof(confdata)/sizeof(*confdata));
}