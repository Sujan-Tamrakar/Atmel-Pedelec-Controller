#include "asf.h"
#include "can_com.h"

//number of receive mobs for each channel to handle concurrently receiving data on can bus
#define CAN_COM_NUM_OF_RECEIVE_MOBS_PER_CH 4

can_msg_t mob_ram_ch0[NB_MOB_CHANNEL];
can_msg_t mob_ram_ch1[NB_MOB_CHANNEL];

can_msg_t msg_tx = {{{0,0x1ff}, },0x0LL};
can_msg_t msg_rx = {{{0,0}, },0x0LL};

//stores handles, which are marked as receiving handles to distinguish callback types (tx/rx)
U32 tx_handle_vector_ch0 = 0;
U32 tx_handle_vector_ch1 = 0;

void (*can_com_tx_callback_channel0) (U8 handle, U8 event);
void (*can_com_tx_callback_channel1) (U8 handle, U8 event);
void (*can_com_rx_callback_channel0) (U8 handle, U8 event);
void (*can_com_rx_callback_channel1) (U8 handle, U8 event);

void can_com_set_tx_callback(U8 ch, void (*can_com_callback) (U8 handle, U8 event)) {
	if(ch == 0) {
		can_com_tx_callback_channel0 = can_com_callback;
	} else {
		can_com_tx_callback_channel1 = can_com_callback;
	}
}

void can_com_set_rx_callback(U8 ch, void (*can_com_callback) (U8 handle, U8 event)) {
	if(ch == 0) {
		can_com_rx_callback_channel0 = can_com_callback;
	} else {
		can_com_rx_callback_channel1 = can_com_callback;
	}
}

void can_com_callback_wrapper_channel0(U8 handle, U8 event)
{	
	if(tx_handle_vector_ch0 & (1 << handle)) {
		//handle was from tx
		
		if(can_com_tx_callback_channel0 != NULL) {
			can_com_tx_callback_channel0(handle, event);
		}
		tx_handle_vector_ch0 &= ~(1 << handle);
	} else {
		//handle was from rx
		
		if(can_com_rx_callback_channel0 != NULL) {
			can_com_rx_callback_channel0(handle, event);
		}
		can_com_rx(0); //allocate new rx mob
	}
	can_mob_free(0,handle); //free this mob
}

void can_com_callback_wrapper_channel1(U8 handle, U8 event)
{	
	if(tx_handle_vector_ch1 & (1 << handle)) {
		//handle was from tx
		
		if(can_com_tx_callback_channel1 != NULL) {
			can_com_tx_callback_channel1(handle, event);
		}
		tx_handle_vector_ch1 &= ~(1 << handle);
	} else {
		//handle was from rx
				
		if(can_com_rx_callback_channel1 != NULL) {
			can_com_rx_callback_channel1(handle, event);
		}
		can_com_rx(1); //allocate new rx mob
	}
	can_mob_free(1,handle); //free this mob
}

void can_com_clock_init(void)
{
	// Setup and enable generic clock for CAN
	scif_gc_setup(AVR32_SCIF_GCLK_CANIF, SCIF_GCCTRL_OSC0, AVR32_SCIF_GC_NO_DIV_CLOCK, 0);
	scif_gc_enable(AVR32_SCIF_GCLK_CANIF);
	
	// Assign GPIO to CAN.
	static const gpio_map_t CAN_GPIO_MAP = {
		{AVR32_CANIF_RXLINE_1_1_PIN, AVR32_CANIF_RXLINE_1_1_FUNCTION},
		{AVR32_CANIF_TXLINE_1_1_PIN, AVR32_CANIF_TXLINE_1_1_FUNCTION},
		{AVR32_CANIF_RXLINE_0_1_PIN, AVR32_CANIF_RXLINE_0_1_FUNCTION},
		{AVR32_CANIF_TXLINE_0_1_PIN, AVR32_CANIF_TXLINE_0_1_FUNCTION}
	};
	gpio_enable_module(CAN_GPIO_MAP, sizeof(CAN_GPIO_MAP) / sizeof(CAN_GPIO_MAP[0]));
}

void can_com_init(void)
{
	can_com_clock_init();
	
	can_init(0, ((U32)&mob_ram_ch0[0]), CANIF_CHANNEL_MODE_NORMAL, can_com_callback_wrapper_channel0);
	can_init(1, ((U32)&mob_ram_ch1[0]), CANIF_CHANNEL_MODE_NORMAL, can_com_callback_wrapper_channel1);
	
	//reserve several receive mobs
	for(U8 i = 0; i < CAN_COM_NUM_OF_RECEIVE_MOBS_PER_CH; i++) {
		can_com_rx(0);
		can_com_rx(1);
	}
}

U8 can_com_tx(U8 ch, U64 id, U64 data, U8 dlc)
{
	irqflags_t irqflags = cpu_irq_save();

	msg_tx.data.u64 = data;
	msg_tx.id = id;
	
	can_mob_t can_mob_message_tx = {CAN_MOB_NOT_ALLOCATED,NULL,dlc,CAN_DATA_FRAME,CAN_STATUS_NOT_COMPLETED};
	
	can_mob_message_tx.handle = can_mob_alloc(ch);
	can_mob_message_tx.can_msg = &msg_tx;
	
	if (can_mob_message_tx.handle==CAN_CMD_REFUSED) {
		return CAN_CMD_REFUSED;
	}

	// mark handle as tx
	if(ch == 0) {
		tx_handle_vector_ch0 |= 1 << can_mob_message_tx.handle; 
	} else {
		tx_handle_vector_ch1 |= 1 << can_mob_message_tx.handle;
	}

	U8 result = can_tx(ch, can_mob_message_tx.handle, can_mob_message_tx.dlc, can_mob_message_tx.req_type, can_mob_message_tx.can_msg);
	
	cpu_irq_restore(irqflags);
	return result;
}

U8 can_com_rx(U8 ch)
{
	irqflags_t irqflags = cpu_irq_save();

	can_mob_t can_mob_message_rx = {{CAN_MOB_NOT_ALLOCATED,NULL,0,CAN_DATA_FRAME,CAN_STATUS_NOT_COMPLETED}};
	
	can_mob_message_rx.handle = can_mob_alloc(ch);
	can_mob_message_rx.can_msg = &msg_rx;
	
	if (can_mob_message_rx.handle==CAN_CMD_REFUSED) {
		return CAN_CMD_REFUSED;
	}

	U8 result = can_rx(ch, can_mob_message_rx.handle, can_mob_message_rx.req_type, can_mob_message_rx.can_msg);
	
	cpu_irq_restore(irqflags);
	return result;
}