/*
 * ble_aci.h
 *
 * Created: 30-Sep-14 8:18:52 AM
 *  Author: dploetzer
 */ 


#ifndef BLE_ACI_H_
#define BLE_ACI_H_

#define ACI_TEST_SPI_TIMEOUT_COUNT 10 // * 100ms

typedef enum
{
	ACI_DISCONNECT_REASON_USER = 0x01,
	ACI_DISCONNECT_REASON_TIMING = 0x02
} ble_aci_disconnect_reason;

typedef enum
{
	ACI_OPERATING_MODE_TEST = 0x01,
	ACI_OPERATING_MODE_SETUP = 0x02,
	ACI_OPERATING_MODE_STANDBY = 0x03
} ble_aci_operating_mode;

typedef enum
{
	ACI_INIT_OK,
	ACI_INIT_DEVICE_START_ERROR,
	ACI_INIT_HW_ERROR
} ble_aci_init_result;

typedef enum
{
	ACI_TEST_OK,
	ACI_TEST_ENTER_ERROR,
	ACI_TEST_EXIT_ERROR,
	ACI_TEST_HW_ERROR,
	ACI_TEST_ECHO_ERROR,
} ble_aci_test_result;

typedef enum
{
	ACI_COMMAND_UNKNOWN = 0x00,
	ACI_COMMAND_TEST = 0x01,
	ACI_COMMAND_ECHO = 0x02,
	ACI_COMMAND_SETUP = 0x06,
	ACI_COMMAND_SET_LOCAL_DATA = 0x0D,
	ACI_COMMAND_CONNECT = 0x0F,
	ACI_COMMAND_DISCONNECT = 0x11,
	ACI_COMMAND_OPEN_REMOTE_PIPE = 0x14,
	ACI_COMMAND_SEND_DATA = 0x15,
	ACI_COMMAND_CLOSE_REMOTE_PIPE = 0x1F,
	ACI_DEVICE_STARTED_EVENT = 0x81,
	ACI_ECHO_EVENT = 0x82,
	ACI_COMMAND_RESPONSE_EVENT = 0x84,
	ACI_CONNECTED_EVENT = 0x85,
	ACI_DISCONNECTED_EVENT = 0x86,
	ACI_PIPE_STATUS_EVENT = 0x88,
	ACI_DATA_CREDIT_EVENT = 0x8A,
	ACI_DATA_ACK_EVENT = 0x8B,
	ACI_DATA_RECEIVE_EVENT = 0x8C,
	ACI_PIPE_ERROR_EVENT = 0x8D
} ble_aci_type;

#pragma pack(1)
typedef union
{
	uint8_t raw[32];
	
	struct {
		uint8_t TestFeature;
	} TestCommand; //0x01
	
	struct {
		uint8_t data[30];
	} EchoCommand; //0x02
	
	struct {
		uint8_t ServicePipeNumber;
		uint8_t Data[20];
	} SetLocalDataCommand; //0x06
	
	struct {
		uint8_t data[30];
	} SetupCommand; //0x0D
	
	struct {
		uint16_t Timeout;
		uint16_t AdvInternal;
	} ConnectCommand; //0x0F
	
	struct {
		uint8_t Reason;
	} DisconnectCommand; //0x11
	
	struct {
		uint8_t ServicePipeNumber;
	} OpenRemotePipeCommand; //0x14
	
	struct {
		uint8_t ServicePipeNo;
		uint8_t Data[20];
	} SendData; //0x15
	
	struct {
		uint8_t ServicePipeNumber;
	} CloseRemotePipeCommand; //0x1F
	
	struct {
		uint8_t OperatingMode;
		uint8_t HWError;
		uint8_t DataCreditAvailable;
	} DeviceStartedEvent; //0x81
	
	struct {
		uint8_t data[30];
	} EchoEvent; //0x82
	
	struct {
		ble_aci_type CommandOpCode;
		uint8_t status;
		uint8_t data[28];
	} CommandResponseEvent; //0x84
	
	struct {
		uint8_t AddressType;
		uint8_t PeerAdress[6];
		uint16_t ConnectionInterval;
		uint16_t SlaveLatency;
		uint16_t SupervisionTimeout;
		uint8_t MasterClockAccurcy;
	} ConnectedEvent; //0x85
	
	struct {
		uint8_t AciStatus;
		uint8_t BtLeStatus;
	} DisonnectedEvent; //0x85
	
	struct {
		uint8_t PipesOpen[8];
		uint8_t PipesClosed[8];
	} PipeStatusEvent; //0x88
	
	struct {
		uint8_t DataCredits;
	} DataCreditEvent; //0x8A
	
	struct {
		uint8_t ServicePipeNumber;	
	} DataAckEvent; //0x8B
	
	struct {
		uint8_t ServicePipeNo;
		Union64 Data[20];
	} DataReceiveEvent; //0x8C
	
	struct {
		uint8_t ServicePipeNo;
		uint8_t ErrorCode;
		uint8_t ErrorData[27];
	} PipeErrorEvent; //0x8D
	
} ble_aci_content;


typedef struct  
{
	uint8_t len;
	ble_aci_type type;
	ble_aci_content content;
} ble_aci_packet;

void ble_aci_read_packet(void);
void ble_aci_write_packet(void);

ble_aci_init_result ble_aci_init(void);
ble_aci_test_result ble_aci_test_device(void);
void ble_aci_setup(void);
void ble_aci_connect(void);
void ble_aci_disconnect(ble_aci_disconnect_reason reason);
void ble_aci_send(uint8_t pipe, uint8_t* buffer, uint8_t len);
void ble_aci_set_local_data(uint8_t pipe, uint8_t* buffer, uint8_t len);
void ble_aci_open_pipe(uint8_t pipe);
void ble_aci_close_pipe(uint8_t pipe);

//global variables
ble_aci_packet ble_aci_packet_rx;
ble_aci_packet ble_aci_packet_tx;

#endif /* BLE_ACI_H_ */