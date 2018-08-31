#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Atmel Software Framework header files */
#include "asf.h"

/* Controller header files */
#include "ble_spi.h"
#include "ble_aci.h"
#include "ble_aci_setup.h"
#include "ble_task.h"
#include "can_com.h"
#include "can_buffer.h"
#include "timer.h"
#include "hmi.h"
#include "can_task.h"


/* Scheduler header files */
#include "FreeRTOS.h"
#include "task.h"

/* Defines */
#define mainBLE_TASK_PRIORITY ( tskIDLE_PRIORITY + 1 )
#define mainCAN_TASK_PRIORITY ( tskIDLE_PRIORITY + 1 )

int main( void )
{
	pcl_switch_to_osc(PCL_OSC0, FOSC0, OSC0_STARTUP);

	// Initialize USART for stdio debugging
	init_dbg_rs232(FPBA_HZ);
	print_dbg(CURSOFF CLEARSCR);
		
	irq_initialize_vectors();
	cpu_irq_enable();
	
	//init & start timer
	timer_init();
	timer_start();

	vStartCANTask( mainCAN_TASK_PRIORITY );
	vStartBLETask( mainBLE_TASK_PRIORITY );
	
	vTaskStartScheduler();

	return 0;
}