#ifndef CAN_BUFFER_H_
#define CAN_BUFFER_H_

#define CAN_BUFFER_LEN 1024

typedef enum {
	CAN_BUFFER_DATA_DIR_RX = 0,
	CAN_BUFFER_DATA_DIR_TX = 1
} data_direction_t;

#pragma pack(1) //without this pragma the size of the struct rounds up to n times 8 byte (32 bit)
typedef struct {
	U32 ms;
	U32 id;
	U8 ch : 1;
	data_direction_t dir : 1;
	U8 dlc : 6;
	U64 data;
} can_datapacket_t;

typedef struct {
	volatile U32 head;
	volatile U32 tail;
	can_datapacket_t datapacket[CAN_BUFFER_LEN];
} can_buffer_t;

void can_buffer_init(void);
void can_buffer_add(can_datapacket_t datapacket);
can_datapacket_t can_buffer_remove(void);
bool can_buffer_has_next();

#endif /* CAN_BUFFER_H_ */