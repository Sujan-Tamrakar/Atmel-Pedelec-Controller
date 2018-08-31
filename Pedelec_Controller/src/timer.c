#include "asf.h"
#include "timer.h"

volatile U32 timer_count_ms;

void (*timer_callback0) (U32 ms);
void (*timer_callback1) (U32 ms);

/*! \brief TC interrupt.
 */
__attribute__((__interrupt__))
static void timer_interrupt(void)
{
	// Clear the interrupt flag. This is a side effect of reading the TC SR.
	tc_read_sr(TC_INSTANCE, TC_CHANNEL);
	
	timer_count_ms++;
	
	if(timer_callback0 != NULL) {
		timer_callback0(timer_count_ms);
	}
	if(timer_callback1 != NULL) {
		timer_callback1(timer_count_ms);
	}
}

void timer_set_callback(U8 callback_no, void (*callback)(U32 ms)) {
	switch(callback_no) {
		case 0: timer_callback0 = callback; break;
		case 1: timer_callback1 = callback; break;
	}
}

volatile avr32_tc_t *tc = TC_INSTANCE;

void timer_init(void)
{	
	// Options for waveform generation.
	static const tc_waveform_opt_t WAVEFORM_OPT = {
		.channel  = TC_CHANNEL,         // Channel selection.
		.bswtrg   = TC_EVT_EFFECT_NOOP, // Software trigger effect on TIOB.
		.beevt    = TC_EVT_EFFECT_NOOP, // External event effect on TIOB.
		.bcpc     = TC_EVT_EFFECT_NOOP, // RC compare effect on TIOB.
		.bcpb     = TC_EVT_EFFECT_NOOP, // RB compare effect on TIOB.
		.aswtrg   = TC_EVT_EFFECT_NOOP, // Software trigger effect on TIOA.
		.aeevt    = TC_EVT_EFFECT_NOOP, // External event effect on TIOA.
		.acpc     = TC_EVT_EFFECT_NOOP, // RC compare effect on TIOA: toggle.
		.acpa     = TC_EVT_EFFECT_NOOP, // RA compare effect on TIOA: toggle.
		.wavsel   = TC_WAVEFORM_SEL_UP_MODE_RC_TRIGGER, // Waveform selection: Up mode with automatic trigger(reset) on RC compare.
		.enetrg   = false, // External event trigger enable.
		.eevt     = 0, // External event selection.
		.eevtedg  = TC_SEL_NO_EDGE,    // External event edge selection.
		.cpcdis   = false,             // Counter disable when RC compare.
		.cpcstop  = false,             // Counter clock stopped with RC compare.
		.burst    = false,             // Burst signal selection.
		.clki     = false,             // Clock inversion.
		.tcclks   = TC_CLOCK_SOURCE_TC3 // Internal source clock 3, connected to FPBA_HZ / 8.
	};

	static const tc_interrupt_t TC_INTERRUPT = {
		.etrgs = 0,
		.ldrbs = 0,
		.ldras = 0,
		.cpcs  = 1,
		.cpbs  = 0,
		.cpas  = 0,
		.lovrs = 0,
		.covfs = 0
	};
	// Register the TC interrupt handler to the interrupt controller.
	INTC_register_interrupt(&timer_interrupt, TC_INSTANCE_IRQ, AVR32_INTC_INT0);
	
	// Initialize the timer/counter.
	tc_init_waveform(tc, &WAVEFORM_OPT);

	// Set the compare triggers.
	// We want: (1 / (FPBA_HZ / 8)) * RC = 1 ms, hence RC = (FPBA_HZ / 8) / 1000
	tc_write_rc(tc, TC_CHANNEL, (FPBA_HZ / 8) / 1000);

	tc_configure_interrupts(tc, TC_CHANNEL, &TC_INTERRUPT);
}

void timer_start(void) {
	timer_count_ms = 0;
	tc_start(tc, TC_CHANNEL);
}

U32 timer_get_ms(void) {
	return timer_count_ms;
}