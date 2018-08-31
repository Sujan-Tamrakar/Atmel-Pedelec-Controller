#ifndef CAN_COM_H_
#define CAN_COM_H_

U8 can_com_tx(U8 ch, U64 id, U64 data, U8 dlc);
U8 can_com_rx(U8 ch);
void can_com_set_tx_callback( U8 ch, void (*can_msg_callback) (U8,U8));
void can_com_set_rx_callback( U8 ch, void (*can_msg_callback) (U8,U8));
void can_com_init(void);

#endif /* CAN_H_ */