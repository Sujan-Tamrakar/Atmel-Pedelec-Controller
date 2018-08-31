#include <stdlib.h>

#include "asf.h"

/* Scheduler include files */
#include "FreeRTOS.h"
#include "task.h"

/* module include files */
#include "can_com.h"
#include "can_buffer.h"
#include "can_task.h"
#include "timer.h"
#include "hmi.h"

#define canSTACK_SIZE configMINIMAL_STACK_SIZE

/* Debug variables */
volatile U32 buffer_counter = 0;
volatile U32 receive_ch0_counter = 0;
volatile U32 receive_ch1_counter = 0;
volatile U32 send_ch0_counter = 0;
volatile U32 send_ch1_counter = 0;

void receive_callback_ch0(U8 handle, U8 event) {
	receive_ch0_counter++;
			
	gpio_tgl_gpio_pin(LED2_GPIO); 
	
	can_datapacket_t datapacket = {
		.dir = CAN_BUFFER_DATA_DIR_RX,
		.ms = timer_get_ms(),
		.ch = 0,
		.id = can_get_mob_id(0,handle),
		.dlc = can_get_mob_dlc(0,handle),
		.data = can_get_mob_data(0,handle).u64
	};
	
	can_buffer_add(datapacket);
}

void send_callback_ch0(U8 handle, U8 event) {
	send_ch0_counter++;
	
	gpio_tgl_gpio_pin(LED3_GPIO);
	
	// log data
	can_datapacket_t datapacket = {
		.dir = CAN_BUFFER_DATA_DIR_TX,
		.ms = timer_get_ms(),
		.ch = 0,
		.id = can_get_mob_id(0,handle),
		.dlc = can_get_mob_dlc(0,handle),
		.data = can_get_mob_data(0,handle).u64
	};
	can_buffer_add(datapacket);
}

/*-----------------------------------------------------------*/

static portTASK_FUNCTION_PROTO( canTask, pvParameters );

void vStartCANTask( unsigned portBASE_TYPE uxPriority )
{
	xTaskCreate(canTask, ( signed char * ) "CAN", canSTACK_SIZE, NULL, uxPriority, ( xTaskHandle * ) NULL );
}

static portTASK_FUNCTION( canTask, pvParameters )
{
	//print_dbg(OVERWRITEMOD);
	
	//register callbacks
	can_com_set_rx_callback(0, receive_callback_ch0);
	can_com_set_tx_callback(0, send_callback_ch0);
			
	//init can buffer & com
	can_buffer_init();
	can_com_init();
	
	while (1) {
		
		if(can_buffer_has_next()) {
		
			while(can_buffer_has_next()) {
				
				can_datapacket_t datapacket = can_buffer_remove();
				
				//parse incoming data
				if(datapacket.dir == CAN_BUFFER_DATA_DIR_RX) {
					hmi_parse_data(datapacket);
				}

				buffer_counter++;
			}	
		}
	}
}