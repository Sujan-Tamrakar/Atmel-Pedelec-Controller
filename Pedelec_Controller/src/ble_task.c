#include <stdlib.h>

#include "asf.h"

/* Scheduler include files */
#include "FreeRTOS.h"
#include "task.h"

/* module include files */
#include "ble_spi.h"
#include "ble_aci.h"
#include "ble_aci_setup.h"
#include "ble_task.h"
#include "hmi.h"
#include "timer.h"
#include "imu.h"
#include "imu_spi.h"

#define bleSTACK_SIZE configMINIMAL_STACK_SIZE

bool ble_connected = false;

uint8_t* buffer;

void ble_task_transmit_data(U32 ms)
{
	if( ms % 100 == 0 && hmi_is_on() ) {
		
		gpio_tgl_gpio_pin(LED1_GPIO);
					
		U16 speed = hmi_get_speed();
		ble_aci_set_local_data(PIPE_PEDELEC_PEDELEC_SPEED_TX,(uint8_t*) &speed,2); //1100
				
		U8 battery = hmi_get_battery();
		ble_aci_set_local_data(PIPE_PEDELEC_PEDELEC_BATTERY_TX,(uint8_t*) &battery,1); //1101
		
		U32 total_distance = hmi_get_total_distance();
		ble_aci_set_local_data(PIPE_PEDELEC_PEDELEC_TOTAL_DISTANCE_TX,(uint8_t*) &total_distance,4); //1102
		
		U32 total_uptime = hmi_get_total_uptime();
		ble_aci_set_local_data(PIPE_PEDELEC_PEDELEC_TOTAL_UPTIME_TX,(uint8_t*) &total_uptime,4); //1103
		
		U32 reach = hmi_get_reach();
		ble_aci_set_local_data(PIPE_PEDELEC_PEDELEC_REACH_TX,(uint8_t*) &reach,4); //1104
		
		U8 engine_power = hmi_get_engine_power();
		ble_aci_set_local_data(PIPE_PEDELEC_PEDELEC_ENGINE_POWER_TX,(uint8_t*) &engine_power,1); //1105
		
		float velocity = acc_raw_2_velocity();
		ble_aci_set_local_data(PIPE_PEDELEC_PEDELEC_ACCELEROMETER_RAW_DATA_TX,(uint8_t*) &velocity ,1); //1106
		
	}
}

void ble_task_receive_data()
{
	while(ble_spi_has_data()) {
		
		gpio_tgl_gpio_pin(LED0_GPIO);
		
		ble_aci_read_packet();		
					
		print_dbg("Received packet (event: ");
		print_dbg_short_hex(ble_aci_packet_rx.type);
		print_dbg(")" CRLF);
	
		if( ble_aci_packet_rx.type == ACI_DATA_RECEIVE_EVENT ) {
			
			switch( ble_aci_packet_rx.content.DataReceiveEvent.ServicePipeNo ) {
				
				case PIPE_PEDELEC_PEDELEC_TRACTION_AID_RX:
					hmi_set_walk_status(ble_aci_packet_rx.content.DataReceiveEvent.Data->u8[0]);
					break;
				
				case PIPE_PEDELEC_PEDELEC_ASSISTANCE_LEVEL_RX:
					hmi_set_assistance_level(ble_aci_packet_rx.content.DataReceiveEvent.Data->u8[0]);
					break;
			}
						
		} else if(ble_aci_packet_rx.type == ACI_DISCONNECTED_EVENT) {
			ble_connected = false;
		}
	}
}


/*-----------------------------------------------------------*/

static portTASK_FUNCTION_PROTO( bleTask1, pvParameters );

void vStartBLETask( unsigned portBASE_TYPE uxPriority )
{
	xTaskCreate(bleTask1, ( signed char * ) "BLE", bleSTACK_SIZE, NULL, uxPriority, ( xTaskHandle * ) NULL );
}

static portTASK_FUNCTION( bleTask1, pvParameters )
{	
	
	ble_spi_init();
	
	imu_spi_configuration();

	ble_aci_init_result init_status = ble_aci_init();
	if(init_status == ACI_INIT_OK) {
		print_dbg("BLE device init OK!" CRLF);
	} else {
		print_dbg("Device error: ");
		switch(init_status) {
			case ACI_INIT_DEVICE_START_ERROR: print_dbg("Device start error!" CRLF); break;
			case ACI_INIT_HW_ERROR: print_dbg("fatal hardware error!" CRLF); break;
		}
		
		while(1);
	}
	
	ble_aci_test_result device_status = ble_aci_test_device();
	if( device_status == ACI_TEST_OK ) {
		print_dbg("BLE device test OK!" CRLF);
	} else {
		print_dbg("Device error: ");
		switch(device_status) {
			case ACI_TEST_ENTER_ERROR: print_dbg("unable to enter test mode!" CRLF); break;
			case ACI_TEST_EXIT_ERROR: print_dbg("unable to exit test mode!" CRLF); break;
			case ACI_TEST_HW_ERROR: print_dbg("fatal hardware error!" CRLF); break;
			case ACI_TEST_ECHO_ERROR: print_dbg("echo failed!" CRLF); break;
		}
		
		while(1);
	}
	
	ble_aci_setup();
	
	while(1) {
		print_dbg("Waiting for connection..." CRLF);
		ble_aci_connect();
		
		if(ble_aci_packet_rx.type == ACI_CONNECTED_EVENT) {
			
			ble_connected = true;
				
			timer_set_callback(1,ble_task_transmit_data); //register continuous data update to timer
			
			print_dbg("Device connected: ");
			for(uint8_t i = 0; i < 6; i++) {
				print_dbg_char_hex(ble_aci_packet_rx.content.ConnectedEvent.PeerAdress[5-i]);
				if(i < 5) {
					print_dbg_char(':');
				}
			}
			print_dbg(CRLF);
			
			while(ble_connected) {
				ble_task_receive_data();
			}
			
			timer_set_callback(1,NULL); //unregister continuous data update
			print_dbg("Device disconnected!" CRLF);
			
		} else {
			print_dbg("Connection error by event: ");
			print_dbg_short_hex(ble_aci_packet_rx.type);
			print_dbg(CRLF);
		}
	}
}

