/*
 * IMU.c
 *
 * Created: 6/6/2017 4:59:08 PM
 *  Author: Sujan Tamrakar
 */ 

// AT32UC3C Board Configurations

#include <string.h>
#include <sysclk.h>
#include <asf.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "imu_spi.h"
#include "imu.h"
#include <sys/time.h>


#define VREF								(3.3f)    //Accelerometer VREF
#define VZEROG								(1.65f)   //Accelerometer VZEROG
#define SENSITIVITY							(0.4785f) //Accelerometer SENSITIVITY
#define ACCEL_XOUT_H                        0xBB00    // X axis accelerometer offset (high byte)
#define ACCEL_XOUT_L                        0xBC00    // X axis accelerometer offset (low byte)
#define ACCEL_YOUT_H                        0xBD00    // Y axis accelerometer offset (high byte)
#define ACCEL_YOUT_L                        0xBE00    // Y axis accelerometer offset (low byte)
#define ACCEL_ZOUT_H                        0xC000    // Z axis accelerometer offset (high byte)
#define ACCEL_ZOUT_L                        0xC100    // Z axis accelerometer offset (low byte)

uint8_t rxdata_new[6];
unsigned char ACC_Data[6];
float velX = 0, velY = 0, velZ = 0;

float binary_to_decimal_converter(char *bits){
	float ret = 0;
	int i = 1;
	for(i = 1; i < strlen(bits); i++){
		ret += (atof(bits[i]) * pow(2,strlen(bits)- i));
	}
	if(bits[0] == "1")ret = -ret;	
	return ret;
}

float acc_raw_2_velocity(void)
 {
	struct timeval stop, start;
	gettimeofday(&start, NULL);
	hmi_get_acelerometer();
	gettimeofday(&stop, NULL);
	
	long int dt = stop.tv_usec - start.tv_usec;
	
	ACC_Data[0] = rxdata_new[1];//X_Low Byte
	ACC_Data[1] = rxdata_new[0];//X_High_Byte
	
	ACC_Data[2] = rxdata_new[3];//Y_Low_Byte
	ACC_Data[3] = rxdata_new[2];//Y_High_Byte

	ACC_Data[4] = rxdata_new[5];//Z_Low_Byte
	ACC_Data[5] = rxdata_new[4];//Z_High_Byte

	//get raw x, y and z
	char Ax[16], Ay[16], Az[16];
	strcpy(Ax,ACC_Data[0]);
	strcat(Ax,ACC_Data[1]);
	
	strcpy(Ay,ACC_Data[2]);
	strcat(Ay,ACC_Data[3]);
	 
	strcpy(Az,ACC_Data[4]);
	strcat(Az,ACC_Data[5]);
	 
	//convert x,y,z to decimal
	float X = binary_to_decimal_converter(Ax);
	float Y = binary_to_decimal_converter(Ay);
	float Z = binary_to_decimal_converter(Az);
	
	//formula
	float accX = roundf(((X * VREF / (pow(2,16)-1)) - VZEROG)/(SENSITIVITY) * 100.0f)/100.0f;
	float accY = roundf(((Y * VREF / (pow(2,16)-1)) - VZEROG)/(SENSITIVITY) * 100.0f)/100.0f;
	float accZ = roundf(((Z * VREF / (pow(2,16)-1)) - VZEROG)/(SENSITIVITY) * 100.0f)/100.0f;
	
	//update velocity
	velX += (accX * dt);
	velY += (accY * dt);
	velZ += (accZ * dt);
	
	//final velocity
	return (float) sqrt(pow(velX,2) + pow(velY,2) + pow(velZ,2));
}

//uint8_t* hmi_get_acelerometer(void)
void hmi_get_acelerometer(void)
{
	int i = 0;
	for (i = 0; i<6; i++){
		rxdata_new[i] =0;
	}
	
	//Buffer to send data to SPI slave
	uint16_t txdata;
	//Buffer to receive data from SPI slave
	uint16_t rxdata;



	//Reading accelerometer x-axis, y-axis, z-axis data
	
	// X-AXIS
	// Send the data to slave (ie = AT45DBX_CMDC_RD_STATUS_REG)
	txdata=ACCEL_XOUT_H;
	//spi_write(IMU_SPI,0x1C);
		
	//Select given device on the SPI bus
	spi_selectChip(IMU_SPI, SPI_SLAVECHIP_NUMBER);
		
	//return reverse_byte((uint8_t)rxdata);
	spi_put(IMU_SPI,txdata);
	//Wait for a complete transmission
		
	while(!spi_is_tx_empty(IMU_SPI));
		
	spi_read(IMU_SPI,&rxdata);
	//Now simply read the data in the receive register
	rxdata_new[0]=spi_get(IMU_SPI);
		
	// Deselect the slave
	spi_unselectChip(IMU_SPI,SPI_SLAVECHIP_NUMBER);
		
	// Send the data to slave (ie = AT45DBX_CMDC_RD_STATUS_REG)
	txdata=ACCEL_XOUT_L;
	//spi_write(IMU_SPI,0x1C);
		
	//Select given device on the SPI bus
	spi_selectChip(IMU_SPI, SPI_SLAVECHIP_NUMBER);
		
	//return reverse_byte((uint8_t)rxdata);
	spi_put(IMU_SPI,txdata);
	//Wait for a complete transmission
	while(!spi_is_tx_empty(IMU_SPI));
		
	spi_read(IMU_SPI,&rxdata);
	//Now simply read the data in the receive register
	rxdata_new[1]=spi_get(IMU_SPI);
		
	// Deselect the slave
	spi_unselectChip(IMU_SPI,SPI_SLAVECHIP_NUMBER);
		
		
		
	// Y-AXIS
	// Send the data to slave (ie = AT45DBX_CMDC_RD_STATUS_REG)
	txdata=ACCEL_YOUT_H;
	//spi_write(IMU_SPI,0x1C);
		
	//Select given device on the SPI bus
	spi_selectChip(IMU_SPI, SPI_SLAVECHIP_NUMBER);
		
	//return reverse_byte((uint8_t)rxdata);
	spi_put(IMU_SPI,txdata);
	//Wait for a complete transmission
	while(!spi_is_tx_empty(IMU_SPI));
		
	spi_read(IMU_SPI,&rxdata);
	//Now simply read the data in the receive register
	rxdata_new[2]=spi_get(IMU_SPI);
		
	// Deselect the slave
	spi_unselectChip(IMU_SPI,SPI_SLAVECHIP_NUMBER);
		
		
	// Send the data to slave (ie = AT45DBX_CMDC_RD_STATUS_REG)
	txdata=ACCEL_YOUT_L;
	//spi_write(IMU_SPI,0x1C);
		
	//Select given device on the SPI bus
	spi_selectChip(IMU_SPI, SPI_SLAVECHIP_NUMBER);
		
	//return reverse_byte((uint8_t)rxdata);
	spi_put(IMU_SPI,txdata);
	//Wait for a complete transmission
	while(!spi_is_tx_empty(IMU_SPI));
		
	spi_read(IMU_SPI,&rxdata);
	//Now simply read the data in the receive register
	rxdata_new[3]=spi_get(IMU_SPI);
		
		
		
	// Z-AXIS
	
	// Deselect the slave
	spi_unselectChip(IMU_SPI,SPI_SLAVECHIP_NUMBER);
	
	// Send the data to slave (ie = AT45DBX_CMDC_RD_STATUS_REG)
	txdata=ACCEL_ZOUT_H;
	//spi_write(IMU_SPI,0x1C);
		
	//Select given device on the SPI bus
	spi_selectChip(IMU_SPI, SPI_SLAVECHIP_NUMBER);
		
	//return reverse_byte((uint8_t)rxdata);
	spi_put(IMU_SPI,txdata);
	//Wait for a complete transmission
	while(!spi_is_tx_empty(IMU_SPI));
		
	spi_read(IMU_SPI,&rxdata);
	//Now simply read the data in the receive register
	rxdata_new[4]=spi_get(IMU_SPI);
		
	// Deselect the slave
	spi_unselectChip(IMU_SPI,SPI_SLAVECHIP_NUMBER);
	
	// Send the data to slave (ie = AT45DBX_CMDC_RD_STATUS_REG)
	txdata=ACCEL_ZOUT_L;
	//spi_write(IMU_SPI,0x1C);
		
	//Select given device on the SPI bus
	spi_selectChip(IMU_SPI, SPI_SLAVECHIP_NUMBER);
		
	//return reverse_byte((uint8_t)rxdata);
	spi_put(IMU_SPI,txdata);
	//Wait for a complete transmission
	while(!spi_is_tx_empty(IMU_SPI));
		
	spi_read(IMU_SPI,&rxdata);
	//Now simply read the data in the receive register
	rxdata_new[5]=spi_get(IMU_SPI);
		
	// Deselect the slave
	spi_unselectChip(IMU_SPI,SPI_SLAVECHIP_NUMBER);	
}

