#ifndef HMI_H_
#define HMI_H_

typedef enum {
	HMI_WALK_STATUS_ON=0x01,
	HMI_WALK_STATUS_OFF=0x00
} hmi_walk_status_t;

typedef enum {
	HMI_ASSISTANCE_LVL_TURBO=0x04,
	HMI_ASSISTANCE_LVL_SPORT=0x03,
	HMI_ASSISTANCE_LVL_TOUR=0x02,
	HMI_ASSISTANCE_LVL_ECO=0x01,
	HMI_ASSISTANCE_LVL_OFF=0x09
} assistance_level_t;

/*
 * HMI setter
 */
void hmi_set_walk_status(hmi_walk_status_t);
void hmi_set_assistance_level(assistance_level_t);

/*
 * HMI getter
 */
bool hmi_is_on(void);
U16 hmi_get_speed(void);
U8 hmi_get_engine_power(void);
U8 hmi_get_battery(void);
U32 hmi_get_total_distance(void);
U32 hmi_get_reach(void);
U32 hmi_get_total_uptime(void);

/*
 * parser
 */
void hmi_parse_data(can_datapacket_t);

#endif /* HMI_H_ */