#include "asf.h"
#include "can_buffer.h"

can_buffer_t can_buffer;

void can_buffer_init(void)
{
	//init buffer
	can_buffer.head = 0;
	can_buffer.tail = 0;
}

void can_buffer_add(can_datapacket_t datapacket)
{
	can_buffer.datapacket[can_buffer.head] = datapacket;
	
	can_buffer.head++;
	if(can_buffer.head == CAN_BUFFER_LEN)
		can_buffer.head = 0;
}

can_datapacket_t can_buffer_remove()
{
	can_datapacket_t datapaket = can_buffer.datapacket[can_buffer.tail];
	
	can_buffer.tail++;
	if(can_buffer.tail == CAN_BUFFER_LEN)
		can_buffer.tail = 0;
		
	return datapaket;
}

bool can_buffer_has_next()
{
	return can_buffer.head != can_buffer.tail;
}