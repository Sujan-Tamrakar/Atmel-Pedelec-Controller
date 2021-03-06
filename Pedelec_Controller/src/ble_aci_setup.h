/*
 * nrf_setup_messages_content.h
 *
 * Created: 29.09.2014 16:38:15
 *  Author: dp517616
 */ 

#ifndef NRF_SETUP_MESSAGES_CONTENT_H_
#define NRF_SETUP_MESSAGES_CONTENT_H_

typedef struct {
	uint8_t status_byte;
	uint8_t buffer[32];
} setup_msg_data_t;

/* AUTO GENERATED DEFINES BELOW */

/* Service: Pedelec - Characteristic: Pedelec Speed - Pipe: TX */
#define PIPE_PEDELEC_PEDELEC_SPEED_TX          1
#define PIPE_PEDELEC_PEDELEC_SPEED_TX_MAX_SIZE 2

/* Service: Pedelec - Characteristic: Pedelec Battery - Pipe: TX */
#define PIPE_PEDELEC_PEDELEC_BATTERY_TX          2
#define PIPE_PEDELEC_PEDELEC_BATTERY_TX_MAX_SIZE 1

/* Service: Pedelec - Characteristic: Pedelec Total Distance - Pipe: TX */
#define PIPE_PEDELEC_PEDELEC_TOTAL_DISTANCE_TX          3
#define PIPE_PEDELEC_PEDELEC_TOTAL_DISTANCE_TX_MAX_SIZE 4

/* Service: Pedelec - Characteristic: Pedelec Total Uptime - Pipe: TX */
#define PIPE_PEDELEC_PEDELEC_TOTAL_UPTIME_TX          4
#define PIPE_PEDELEC_PEDELEC_TOTAL_UPTIME_TX_MAX_SIZE 4

/* Service: Pedelec - Characteristic: Pedelec Reach - Pipe: TX */
#define PIPE_PEDELEC_PEDELEC_REACH_TX          5
#define PIPE_PEDELEC_PEDELEC_REACH_TX_MAX_SIZE 4

/* Service: Pedelec - Characteristic: Pedelec Engine Power - Pipe: TX */
#define PIPE_PEDELEC_PEDELEC_ENGINE_POWER_TX          6
#define PIPE_PEDELEC_PEDELEC_ENGINE_POWER_TX_MAX_SIZE 1

/* Service: Pedelec - Characteristic: Pedelec Traction Aid - Pipe: RX */
#define PIPE_PEDELEC_PEDELEC_TRACTION_AID_RX          7
#define PIPE_PEDELEC_PEDELEC_TRACTION_AID_RX_MAX_SIZE 1

/* Service: Pedelec - Characteristic: Pedelec Assistance Level - Pipe: RX */
#define PIPE_PEDELEC_PEDELEC_ASSISTANCE_LEVEL_RX          8
#define PIPE_PEDELEC_PEDELEC_ASSISTANCE_LEVEL_RX_MAX_SIZE 1

/* Service: Pedelec - Characteristic: Pedelec Wheel Scope - Pipe: RX */
#define PIPE_PEDELEC_PEDELEC_WHEEL_SCOPE_RX          9
#define PIPE_PEDELEC_PEDELEC_WHEEL_SCOPE_RX_MAX_SIZE 2

/* Service: Pedelec - Characteristic: Pedelec Accelerometer Raw Data - Pipe: TX */
#define PIPE_PEDELEC_PEDELEC_ACCELEROMETER_RAW_DATA_TX          10
#define PIPE_PEDELEC_PEDELEC_ACCELEROMETER_RAW_DATA_TX_MAX_SIZE 1


#define NUMBER_OF_PIPES 10

#define SERVICES_PIPE_TYPE_MAPPING_CONTENT {\
  {ACI_STORE_LOCAL, ACI_TX},   \
  {ACI_STORE_LOCAL, ACI_TX},   \
  {ACI_STORE_LOCAL, ACI_TX},   \
  {ACI_STORE_LOCAL, ACI_TX},   \
  {ACI_STORE_LOCAL, ACI_TX},   \
  {ACI_STORE_LOCAL, ACI_TX},   \
  {ACI_STORE_LOCAL, ACI_RX},   \
  {ACI_STORE_LOCAL, ACI_RX},   \
  {ACI_STORE_LOCAL, ACI_RX},   \
  {ACI_STORE_LOCAL, ACI_TX},   \
}

#define GAP_PPCP_MAX_CONN_INT 0x6 /**< Maximum connection interval as a multiple of 1.25 msec , 0xFFFF means no specific value requested */
#define GAP_PPCP_MIN_CONN_INT  0x6 /**< Minimum connection interval as a multiple of 1.25 msec , 0xFFFF means no specific value requested */
#define GAP_PPCP_SLAVE_LATENCY 0
#define GAP_PPCP_CONN_TIMEOUT 0xffff /** Connection Supervision timeout multiplier as a multiple of 10msec, 0xFFFF means no specific value requested */

#define NB_SETUP_MESSAGES 33
#define SETUP_MESSAGES_CONTENT {\
    {0x00,\
        {\
            0x07,0x06,0x00,0x00,0x03,0x02,0x42,0x07,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x09,0x01,0x01,0x00,0x00,0x06,0x00,0x01,\
            0xd0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x10,0x1c,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
            0x00,0x00,0x00,0x14,0x00,0x00,0x00,0x14,0x03,0x90,0x01,0xff,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x10,0x38,0xff,0xff,0x02,0x58,0x0a,0x05,0x00,0x00,0x00,0x14,0x00,0x00,0x00,0x14,0x00,0x00,\
            0x00,0x14,0x00,0x00,0x00,0x14,0x00,0x00,0x00,0x00,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x05,0x06,0x10,0x54,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x00,0x04,0x04,0x02,0x02,0x00,0x01,0x28,0x00,0x01,0x00,0x18,0x04,0x04,0x05,0x05,0x00,\
            0x02,0x28,0x03,0x01,0x02,0x03,0x00,0x00,0x2a,0x04,0x04,0x14,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x1c,0x07,0x00,0x03,0x2a,0x00,0x01,0x50,0x65,0x64,0x65,0x6c,0x65,0x63,0x65,0x6d,0x69,\
            0x2e,0x63,0x6f,0x6d,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x04,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x38,0x05,0x05,0x00,0x04,0x28,0x03,0x01,0x02,0x05,0x00,0x01,0x2a,0x06,0x04,0x03,0x02,\
            0x00,0x05,0x2a,0x01,0x01,0x00,0x00,0x04,0x04,0x05,0x05,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x54,0x06,0x28,0x03,0x01,0x02,0x07,0x00,0x04,0x2a,0x06,0x04,0x09,0x08,0x00,0x07,0x2a,\
            0x04,0x01,0x06,0x00,0x06,0x00,0x00,0x00,0xff,0xff,0x04,0x04,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x70,0x02,0x02,0x00,0x08,0x28,0x00,0x01,0x01,0x18,0x04,0x04,0x10,0x10,0x00,0x09,0x28,\
            0x00,0x01,0x4e,0x45,0x48,0x43,0x41,0x41,0x48,0x54,0x57,0x52,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0x8c,0x11,0x49,0x00,0x10,0x00,0x00,0x04,0x04,0x13,0x13,0x00,0x0a,0x28,0x03,0x01,0x12,\
            0x0b,0x00,0x4e,0x45,0x48,0x43,0x41,0x41,0x48,0x54,0x57,0x52,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0xa8,0x11,0x49,0x00,0x11,0x00,0x00,0x16,0x04,0x03,0x02,0x00,0x0b,0x11,0x00,0x02,0x00,\
            0x00,0x46,0x14,0x03,0x02,0x00,0x0c,0x29,0x02,0x01,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0xc4,0x04,0x04,0x13,0x13,0x00,0x0d,0x28,0x03,0x01,0x12,0x0e,0x00,0x4e,0x45,0x48,0x43,\
            0x41,0x41,0x48,0x54,0x57,0x52,0x11,0x49,0x01,0x11,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0xe0,0x16,0x04,0x02,0x01,0x00,0x0e,0x11,0x01,0x02,0x00,0x46,0x14,0x03,0x02,0x00,0x0f,\
            0x29,0x02,0x01,0x00,0x00,0x04,0x04,0x13,0x13,0x00,0x10,0x28,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x20,0xfc,0x03,0x01,0x12,0x11,0x00,0x4e,0x45,0x48,0x43,0x41,0x41,0x48,0x54,0x57,0x52,0x11,\
            0x49,0x02,0x11,0x00,0x00,0x16,0x04,0x05,0x04,0x00,0x11,0x11,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x21,0x18,0x02,0x02,0x00,0x00,0x00,0x00,0x46,0x14,0x03,0x02,0x00,0x12,0x29,0x02,0x01,0x00,\
            0x00,0x04,0x04,0x13,0x13,0x00,0x13,0x28,0x03,0x01,0x12,0x14,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x21,0x34,0x00,0x4e,0x45,0x48,0x43,0x41,0x41,0x48,0x54,0x57,0x52,0x11,0x49,0x03,0x11,0x00,\
            0x00,0x16,0x04,0x05,0x04,0x00,0x14,0x11,0x03,0x02,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x21,0x50,0x00,0x00,0x46,0x14,0x03,0x02,0x00,0x15,0x29,0x02,0x01,0x00,0x00,0x04,0x04,0x13,\
            0x13,0x00,0x16,0x28,0x03,0x01,0x12,0x17,0x00,0x4e,0x45,0x48,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x21,0x6c,0x43,0x41,0x41,0x48,0x54,0x57,0x52,0x11,0x49,0x04,0x11,0x00,0x00,0x16,0x04,0x05,\
            0x04,0x00,0x17,0x11,0x04,0x02,0x00,0x00,0x00,0x00,0x46,0x14,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x21,0x88,0x03,0x02,0x00,0x18,0x29,0x02,0x01,0x00,0x00,0x04,0x04,0x13,0x13,0x00,0x19,0x28,\
            0x03,0x01,0x12,0x1a,0x00,0x4e,0x45,0x48,0x43,0x41,0x41,0x48,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x21,0xa4,0x54,0x57,0x52,0x11,0x49,0x05,0x11,0x00,0x00,0x16,0x04,0x02,0x01,0x00,0x1a,0x11,\
            0x05,0x02,0x00,0x46,0x14,0x03,0x02,0x00,0x1b,0x29,0x02,0x01,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x21,0xc0,0x00,0x00,0x04,0x04,0x13,0x13,0x00,0x1c,0x28,0x03,0x01,0x06,0x1d,0x00,0x4e,0x45,\
            0x48,0x43,0x41,0x41,0x48,0x54,0x57,0x52,0x11,0x49,0x00,0x12,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x21,0xdc,0x00,0x00,0x46,0x14,0x02,0x01,0x00,0x1d,0x12,0x00,0x02,0x00,0x04,0x04,0x13,0x13,\
            0x00,0x1e,0x28,0x03,0x01,0x06,0x1f,0x00,0x4e,0x45,0x48,0x43,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x21,0xf8,0x41,0x41,0x48,0x54,0x57,0x52,0x11,0x49,0x01,0x12,0x00,0x00,0x46,0x14,0x02,0x01,\
            0x00,0x1f,0x12,0x01,0x02,0x04,0x04,0x04,0x13,0x13,0x00,0x20,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x22,0x14,0x28,0x03,0x01,0x06,0x21,0x00,0x4e,0x45,0x48,0x43,0x41,0x41,0x48,0x54,0x57,0x52,\
            0x11,0x49,0x02,0x12,0x00,0x00,0x46,0x14,0x03,0x02,0x00,0x21,\
        },\
    },\
    {0x00,\
        {\
            0x09,0x06,0x22,0x30,0x12,0x02,0x02,0x00,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x40,0x00,0x11,0x00,0x02,0x00,0x02,0x04,0x00,0x0b,0x00,0x0c,0x11,0x01,0x02,0x00,0x02,0x04,\
            0x00,0x0e,0x00,0x0f,0x11,0x02,0x02,0x00,0x02,0x04,0x00,0x11,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x40,0x1c,0x00,0x12,0x11,0x03,0x02,0x00,0x02,0x04,0x00,0x14,0x00,0x15,0x11,0x04,0x02,0x00,\
            0x02,0x04,0x00,0x17,0x00,0x18,0x11,0x05,0x02,0x00,0x02,0x04,\
        },\
    },\
    {0x00,\
        {\
            0x1f,0x06,0x40,0x38,0x00,0x1a,0x00,0x1b,0x12,0x00,0x02,0x00,0x08,0x04,0x00,0x1d,0x00,0x00,0x12,0x01,\
            0x02,0x00,0x08,0x04,0x00,0x1f,0x00,0x00,0x12,0x02,0x02,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x09,0x06,0x40,0x54,0x08,0x04,0x00,0x21,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x13,0x06,0x50,0x00,0x4e,0x45,0x48,0x43,0x41,0x41,0x48,0x54,0x57,0x52,0x11,0x49,0x00,0x00,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x1e,0x06,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
        },\
    },\
    {0x00,\
        {\
            0x06,0x06,0xf0,0x00,0x03,0xdb,0xba,\
        },\
    },\
}

extern setup_msg_data_t setup_message[NB_SETUP_MESSAGES];

#endif /* NRF_SETUP_MESSAGES_CONTENT_H_ */