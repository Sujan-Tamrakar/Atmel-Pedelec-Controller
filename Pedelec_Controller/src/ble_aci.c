/*
 * ble_aci.c
 *
 * Created: 30-Sep-14 8:18:37 AM
 *  Author: dploetzer
 */ 
#include <string.h>
#include <asf.h>
#include "ble_spi.h"
#include "ble_aci.h"
#include "ble_aci_setup.h"
#include "hmi.h"

void ble_aci_read_packet()
{	
	while(!ble_spi_has_data());
					
	irqflags_t irqflags = cpu_irq_save();
	
	ble_spi_selectChip();
	ble_spi_read(); //read debug byte (discard)
	ble_aci_packet_rx.len = ble_spi_read(); //read length byte
	ble_aci_packet_rx.type = ble_spi_read(); //read type byte
	for(uint8_t i = 0; i < ble_aci_packet_rx.len; i++) {
		ble_aci_packet_rx.content.raw[i] = ble_spi_read();
	}
	ble_spi_unselectChip();
	
	cpu_irq_restore(irqflags);
}

void ble_aci_write_packet()
{
	irqflags_t irqflags = cpu_irq_save();
	
	
	/*while( ble_spi_has_data() ) {
		ble_aci_receive_packet();
	}*/
	
	ble_spi_selectChip();
	
	while(!ble_spi_has_data());
		
	ble_spi_write(ble_aci_packet_tx.len);
	ble_spi_write((uint8_t) ble_aci_packet_tx.type);
	for(uint8_t i = 0; i < ble_aci_packet_tx.len; i++) {
		ble_spi_write(ble_aci_packet_tx.content.raw[i]);
	}
	
	ble_spi_unselectChip();
	
	cpu_irq_restore(irqflags);
}

ble_aci_init_result ble_aci_init()
{
	//wait until chip is ready (necessary!)
	while(ble_spi_has_data());
	
	ble_aci_read_packet();
	if(ble_aci_packet_rx.type == ACI_DEVICE_STARTED_EVENT) {
		if(ble_aci_packet_rx.content.DeviceStartedEvent.OperatingMode == ACI_OPERATING_MODE_SETUP) {
			if(ble_aci_packet_rx.content.DeviceStartedEvent.HWError == 0x00) {
				return ACI_INIT_OK;
			}
			return ACI_INIT_HW_ERROR;
		}
	}
	return ACI_INIT_DEVICE_START_ERROR;
}

ble_aci_test_result ble_aci_test_device()
{
	//enter test mode
	ble_aci_packet_tx.type = ACI_COMMAND_TEST;
	ble_aci_packet_tx.len = 2;
	ble_aci_packet_tx.content.TestCommand.TestFeature = 0x02;
	ble_aci_write_packet();

	ble_aci_read_packet();
		
	if(ble_aci_packet_rx.type == ACI_DEVICE_STARTED_EVENT) {
		
		if(ble_aci_packet_rx.content.DeviceStartedEvent.OperatingMode == ACI_OPERATING_MODE_TEST) {

			if(ble_aci_packet_rx.content.DeviceStartedEvent.HWError == 0x00) {
			
				//device ok, try echo...
			
				uint8_t test_byte_1 = 0x44;
				uint8_t test_byte_2 = 0x50;

				ble_aci_packet_tx.type = ACI_COMMAND_ECHO;
				ble_aci_packet_tx.len = 3;
				ble_aci_packet_tx.content.EchoCommand.data[0] = test_byte_1;
				ble_aci_packet_tx.content.EchoCommand.data[1] = test_byte_2;
				ble_aci_write_packet();
			
				ble_aci_read_packet();
			
				if(ble_aci_packet_rx.type == ACI_ECHO_EVENT) {
				
					if( ble_aci_packet_rx.content.EchoEvent.data[0] == test_byte_1 &&
						ble_aci_packet_rx.content.EchoEvent.data[1] == test_byte_2) {
					
						//everything seems to be ok, exit test mode...
						ble_aci_packet_tx.type = ACI_COMMAND_TEST;
						ble_aci_packet_tx.len = 2;
						ble_aci_packet_tx.content.TestCommand.TestFeature = 0xFF;
						ble_aci_write_packet();
					
						ble_aci_read_packet();
				
						if(ble_aci_packet_rx.type == ACI_DEVICE_STARTED_EVENT) {
							if(ble_aci_packet_rx.content.DeviceStartedEvent.OperatingMode == ACI_OPERATING_MODE_SETUP) {
								return ACI_TEST_OK;
							}
						}
					
						return ACI_TEST_EXIT_ERROR;					
					}
				}
				return ACI_TEST_ECHO_ERROR;
			}
			return ACI_TEST_HW_ERROR;
		}
	}
	return ACI_TEST_ENTER_ERROR;
}

void ble_aci_setup()
{	
	for(uint8_t i = 0; i < NB_SETUP_MESSAGES; i++) {
		
		ble_aci_packet_tx.len = setup_message[i].buffer[0];
		ble_aci_packet_tx.type = ACI_COMMAND_SETUP;
		for(uint8_t j = 0; j < ble_aci_packet_tx.len; j++) {
			ble_aci_packet_tx.content.SetupCommand.data[j] = setup_message[i].buffer[j+2];
		}
		
		ble_aci_write_packet();
		ble_aci_read_packet(); //CommandResponseEvent
	}
	ble_aci_read_packet(); //DeviceStartedEvent
}

void ble_aci_connect()
{
	ble_aci_packet_tx.len = 5;
	ble_aci_packet_tx.type = ACI_COMMAND_CONNECT;
	ble_aci_packet_tx.content.ConnectCommand.Timeout = 0x0000;
	ble_aci_packet_tx.content.ConnectCommand.AdvInternal = 0x0020;
	
	ble_aci_write_packet();
	ble_aci_read_packet(); //CommandResponseEvent (discard)
	
	ble_aci_read_packet(); //ConnectedEvent (blocks until connection try)
}

void ble_aci_disconnect(ble_aci_disconnect_reason reason)
{
	ble_aci_packet_tx.len = 2;
	ble_aci_packet_tx.type = ACI_COMMAND_DISCONNECT;
	ble_aci_packet_tx.content.DisconnectCommand.Reason = reason;
	
	ble_aci_write_packet();
	ble_aci_read_packet(); //CommandResponseEvent (discard)
	
	ble_aci_read_packet(); //DisconnectedEvent
}

/*
 * set characteristics
 */
void ble_aci_set_local_data(uint8_t pipe, uint8_t* buffer, uint8_t len)
{
	ble_aci_packet_tx.len = len + 2;
	ble_aci_packet_tx.type = ACI_COMMAND_SET_LOCAL_DATA;
	ble_aci_packet_tx.content.SetLocalDataCommand.ServicePipeNumber = pipe;
	memcpy(ble_aci_packet_tx.content.SetLocalDataCommand.Data, buffer, len);
	ble_aci_write_packet();
	ble_aci_read_packet();
}

void ble_aci_send(uint8_t pipe, uint8_t* buffer, uint8_t len)
{
	ble_aci_packet_tx.len = len + 2;
	ble_aci_packet_tx.type = ACI_COMMAND_SEND_DATA;
	ble_aci_packet_tx.content.SendData.ServicePipeNo = pipe;
	memcpy(ble_aci_packet_tx.content.SendData.Data, buffer, len);
	ble_aci_write_packet();
}

/*
 * open remote pipe
 */
void ble_aci_open_pipe(uint8_t pipe)
{
	ble_aci_packet_tx.len = 2;
	ble_aci_packet_tx.type = ACI_COMMAND_OPEN_REMOTE_PIPE;
	ble_aci_packet_tx.content.OpenRemotePipeCommand.ServicePipeNumber = pipe;
	ble_aci_write_packet();
}

/*
 * close remote pipe
 */
void ble_aci_close_pipe(uint8_t pipe)
{
	ble_aci_packet_tx.len = 2;
	ble_aci_packet_tx.type = ACI_COMMAND_CLOSE_REMOTE_PIPE;
	ble_aci_packet_tx.content.CloseRemotePipeCommand.ServicePipeNumber = pipe;
	ble_aci_write_packet();
}