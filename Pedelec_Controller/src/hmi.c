#include "asf.h"
#include "can_buffer.h"
#include "can_com.h"
#include "timer.h"
#include "hmi.h"
#include <stdio.h>

/*
 * optional enabling for com debugging
 * caution: packages could get lost, because of timing problems by additional writing on COM
 */
//#define HMI_COM_DEBUG

bool hmi_on = false;

/*
 * read only
 */
U16 speed = 0; // 0xD1
U8 engine_power = 0; // 0xD4
U8 battery = 0; // 0x111
U32 total_distance = 0; // 0x202
U32 reach = 0; // 0x202
U32 total_uptime = 0; //0x203

/*
 * write
 */
assistance_level_t assistance_lvl = HMI_ASSISTANCE_LVL_ECO; //3B
hmi_walk_status_t walk_status = HMI_WALK_STATUS_OFF; //0x37


/*
 * HMI getter
 */
bool hmi_is_on()
{
	return hmi_on;
}

U16 hmi_get_speed()
{
	return speed;
}

U8 hmi_get_engine_power()
{
	return engine_power;
}

U8 hmi_get_battery()
{
	return battery;
}

U32 hmi_get_total_distance()
{
	return total_distance;
}

U32 hmi_get_reach()
{
	return reach;
}

U32 hmi_get_total_uptime()
{
	return total_uptime;
}


/*
 * HMI setter
 */
void hmi_set_assistance_level(assistance_level_t lvl)
{
	assistance_lvl = lvl;
}

void hmi_set_walk_status(hmi_walk_status_t status)
{
	walk_status = status;
}

/*
 * local buffer
 */
Union64 data;

#ifdef HMI_COM_DEBUG
void hmi_print_dbg() {
	
	print_dbg("\r");
	
	U8 h = total_uptime / 3600;
	U8 m = (total_uptime - (h * 3600)) / 60;
	U8 s = (total_uptime - (h * 3600) - (m * 60));
	print_dbg("uptime: ");
	print_dbg_ulong(h);
	print_dbg_char(':');
	if(m<10) print_dbg_char('0');
	print_dbg_ulong(m);
	print_dbg_char(':');
	if(s<10) print_dbg_char('0');
	print_dbg_ulong(s);
	print_dbg_char('h');
	
	print_dbg(" | ");
	
	print_dbg("distance: ");
	print_dbg_ulong(total_distance);
	print_dbg("m");

	print_dbg(" | ");
	
	print_dbg("reach: ");
	print_dbg_ulong(reach / 1000);
	print_dbg("km");
	
	print_dbg(" | ");
	
	print_dbg("battery: ");
	print_dbg_ulong(battery);
	print_dbg("%");
	
	print_dbg(" | ");
	
	print_dbg("assistance level: ");
	print_dbg_ulong(assistance_lvl);
	
	print_dbg(" | ");
	
	print_dbg("engine power: ");
	print_dbg_ulong(engine_power);
	print_dbg("%");

	print_dbg(" | ");
	
	print_dbg("speed: ");
	print_dbg_ulong(speed / 100);
	print_dbg_char('.');
	print_dbg_ulong(speed % 100);
	print_dbg("km/h");
	
	print_dbg(CLEARLCR);
}
#endif

void hmi_tx_walk_status() {
	
	/*
	 * beware: can interface masks 8-dlc lowest bytes (instead of highest)!
	 */
	
	if(walk_status == HMI_WALK_STATUS_ON) {
 		can_com_tx(0,0x00000037,0x0000010000000000,3);
	} else if(walk_status == HMI_WALK_STATUS_OFF) {
		can_com_tx(0,0x00000037,0x0000000000000000,3);
	}
}

void hmi_tx_assistance_lvl() {
	
	/*
	 * beware: can interface masks 8-dlc lowest bytes (instead of highest)!
	 */
	
	switch(assistance_lvl)
	{
		case HMI_ASSISTANCE_LVL_TURBO: can_com_tx(0,0x0000003B,0x0400000000000000,4);
		break;
		case HMI_ASSISTANCE_LVL_SPORT: can_com_tx(0,0x0000003B,0x0300000000000000,4);
		break;
		case HMI_ASSISTANCE_LVL_TOUR: can_com_tx(0,0x0000003B,0x0200000000000000,4);
		break;
		case HMI_ASSISTANCE_LVL_ECO: can_com_tx(0,0x0000003B,0x0100000000000000,4);
		break;
		case HMI_ASSISTANCE_LVL_OFF: can_com_tx(0,0x0000003B,0x0900000000000000,4);
		break;
	}
}

volatile U32 Id = 0;

void hmi_tx_cyclic(U32 ms) {
	
	if(ms % 500 == 0) {
		if( assistance_lvl != HMI_ASSISTANCE_LVL_OFF && !gpio_get_pin_value(GPIO_PUSH_BUTTON_0)) {
			switch(assistance_lvl)
			{
				case HMI_ASSISTANCE_LVL_TURBO:
				case HMI_ASSISTANCE_LVL_SPORT:
				case HMI_ASSISTANCE_LVL_TOUR:
					assistance_lvl--;
				break;
				case HMI_ASSISTANCE_LVL_ECO: assistance_lvl = 9;
				break;
			}
		} else if( assistance_lvl != HMI_ASSISTANCE_LVL_TURBO && !gpio_get_pin_value(GPIO_PUSH_BUTTON_1)) {
			switch(assistance_lvl)
			{
				case HMI_ASSISTANCE_LVL_SPORT:
				case HMI_ASSISTANCE_LVL_TOUR:
				case HMI_ASSISTANCE_LVL_ECO:
					assistance_lvl++;
				break;
				case HMI_ASSISTANCE_LVL_OFF: assistance_lvl = 1;
				break;
			}
		}
	}
	
#ifdef HMI_COM_DEBUG
	if(ms % 100 == 0) {
		hmi_print_dbg();
	}
#endif

	if(ms % 100 == 0) {	
		hmi_tx_walk_status();
		hmi_tx_assistance_lvl();
	}
	if(ms % 200 == 0) {
		can_com_tx(0,0x0000009B,0x0000000000000000,1);
	}
	if(ms % 500 == 0) {
		can_com_tx(0,0x00000211,0x0000000000000000,8);
	}
	if(ms % 2000 == 0) {
		can_com_tx(0,0x00000351,0x3008140000000000,8);
	}
}

/*
 * In fact just the packages with identifiers BD and BE are 
 * necessary to keep the motor/battery of the pedelec alive
 */
void hmi_tx_start_sequence() {
	
	/*
	 * beware: can interface masks 8-dlc lowest bytes (instead of highest)!
	 */
	
	can_com_tx(0,0x000000BD,0x0117000000000000,8); //necessary
	can_com_tx(0,0x000000BE,0x050107000C010000,6); //necessary
	can_com_tx(0,0x000000C0,0x4000000000000000,8);
	can_com_tx(0,0x000000C0,0x6000000000000000,8);
	can_com_tx(0,0x0000016A,0x020002FFFF000000,8);
	can_com_tx(0,0x000000BD,0x0117000000000000,8);
	can_com_tx(0,0x000000BE,0x050107000C010000,6);
}


void hmi_parse_data(can_datapacket_t datapacket) {
	
	data.u64 = datapacket.data;
	
	switch (datapacket.id)
	{
		case 0x00000072:
		case 0x00000073:
		case 0x00000080:
		case 0x00000081:
		{
			if(!hmi_on) {
#ifdef HMI_COM_DEBUG
				print_dbg(CLEARSCR "HMI On" CRLF);
#endif				
				hmi_tx_start_sequence();
				timer_set_callback(0,hmi_tx_cyclic);	//register cyclic tx
				hmi_on = true;
			}
		}
		break;
		
		case 0x61:
		{
			if(hmi_on) {
#ifdef HMI_COM_DEBUG
				print_dbg(CLEARSCR "HMI Off");
#endif				
				timer_set_callback(0,NULL);	//unregister cyclic tx
				hmi_on = false;
			}
		}
		break;
		
		case 0xD1: speed = data.u16[0];
		break;
		
		case 0xD4: engine_power = (data.u16[0] * 100) / data.u16[1];
		break;
		
		case 0x111: battery = data.u8[6];
		break;
		
		case 0x202:
		{
			total_distance = data.u32[0];
			reach = data.u32[1];
		}
		break;
		
		case 0x203: total_uptime = data.u32[0];
		break;
		
		break;
	}
}