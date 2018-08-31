#ifndef TIMER_H_
#define TIMER_H_

#define TC_INSTANCE                  &AVR32_TC0
#define TC_INSTANCE_IRQ              AVR32_TC0_IRQ0

// Timer Counter Channel Used
#define TC_CHANNEL                   0

//#define FMCK_HZ     FOSC0//#define FPBA_HZ     FOSC0

void timer_init(void);
void timer_start(void);
U32 timer_get_ms(void);
void timer_set_callback(U8 callback_no, void (*callback)(U32 ms));

#endif /* TIMER_H_ */