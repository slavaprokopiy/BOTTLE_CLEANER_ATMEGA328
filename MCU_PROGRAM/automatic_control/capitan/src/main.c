
#include <asf.h>
#include <vars.h>
#include "adc.h"
#include <string.h>
#include "compiler.h"

extern uint8_t CurrentTimer;
extern uint32_t Timer_Delays;
extern uint16_t Timer_Int_Counter;

volatile uint8_t STATUS_FLAG = 0;
volatile uint8_t RC_STATUS = 0;
volatile uint8_t FLAG = 0;

#define ADC_INPUT      ADC_MUX_ADC0
#define ADC_VREF       ADC_VREF_AVCC



void interrupt_init(void);
void timer_init(void);
void timer_start(uint8_t* Timer_Iteration);
void timer_stop(void);

void debug_led_1_on(void);
void debug_led_1_on(void){
	gpio_set_pin_high(DEBUG_LED_1);
}
void debug_led_2_on(void);
void debug_led_2_on(void){
	gpio_set_pin_high(DEBUG_LED_2);
}
void debug_led_3_on(void);
void debug_led_3_on(void){
	gpio_set_pin_high(DEBUG_LED_3);
}
void debug_led_1_off(void);
void debug_led_1_off(void){
	gpio_set_pin_low(DEBUG_LED_1);
}
void debug_led_2_off(void);
void debug_led_2_off(void){
	gpio_set_pin_low(DEBUG_LED_2);
}
void debug_led_3_off(void);
void debug_led_3_off(void){
	gpio_set_pin_low(DEBUG_LED_3);
}

void do_control_set(int i){
	switch(i){
		case 0:
			gpio_set_pin_high(DO_CONTROL_1);
		break;
		case 1:
			gpio_set_pin_high(DO_CONTROL_2);
		break;
		case 2:
			gpio_set_pin_high(DO_CONTROL_3);
		break;
		case 3:
			gpio_set_pin_high(DO_CONTROL_4);
		break;
		default:
		break;
	}
}
void do_control_clear(int i){
	switch(i){
		case 0:
		gpio_set_pin_low(DO_CONTROL_1);
		break;
		case 1:
		gpio_set_pin_low(DO_CONTROL_2);
		break;
		case 2:
		gpio_set_pin_low(DO_CONTROL_3);
		break;
		case 3:
		gpio_set_pin_low(DO_CONTROL_4);
		break;
		default:
		break;
	}
}
	
uint16_t get_rc_voltage(int i){
	uint16_t result = 0;
	switch(i){
		case 0:
			result = adc_read_10bit(ADC_MUX_ADC4, ADC_VREF_AVCC);// read the voltage from RC-network in loop
		break;
		case 1:
			result = adc_read_10bit(ADC_MUX_ADC5, ADC_VREF_AVCC);// read the voltage from RC-network in loop
		break;
		case 2:
			result = adc_read_10bit(ADC_MUX_ADC6, ADC_VREF_AVCC);// read the voltage from RC-network in loop
		break;
		case 3:
			result = adc_read_10bit(ADC_MUX_ADC7, ADC_VREF_AVCC);// read the voltage from RC-network in loop
		break;
		default:
		break;
	}
	return(result);
}
static uint8_t TIMER_ENABLE_STATUS[4];
uint16_t SELECTOR_MODE[4];
float	SELECTOR_VALUE[4];
uint8_t DELAY[4];



void get_enabled_timers(void);
void get_enabled_timers(void){
	//Getting ENABLE signals
	TIMER_ENABLE_STATUS[0] = ioport_get_pin_level(DI_TIMER_ENABLE_1);
	TIMER_ENABLE_STATUS[1] = ioport_get_pin_level(DI_TIMER_ENABLE_2);
	TIMER_ENABLE_STATUS[2] = ioport_get_pin_level(DI_TIMER_ENABLE_3);
	TIMER_ENABLE_STATUS[3] = ioport_get_pin_level(DI_TIMER_ENABLE_4);
}
#define FIX_MODE	1
#define GALET_MODE	2
#define EXT_MODE	3

void get_selector_modes(void);
void get_selector_modes(void){
	// AI_SELECTOR_TYPE_1	(A4	ADC4/PC4)
	// AI_SELECTOR_TYPE_2	(A5	ADC5/PC5)
	// AI_SELECTOR_TYPE_3	(A6	ADC6)
	// AI_SELECTOR_TYPE_4	(A7	ADC7)
	SELECTOR_MODE[0] = adc_read_10bit(ADC_MUX_ADC4, ADC_VREF_AVCC);
	SELECTOR_MODE[1] = adc_read_10bit(ADC_MUX_ADC5, ADC_VREF_AVCC);
	SELECTOR_MODE[2] = adc_read_10bit(ADC_MUX_ADC6, ADC_VREF_AVCC);
	SELECTOR_MODE[3] = adc_read_10bit(ADC_MUX_ADC7, ADC_VREF_AVCC);
						
	for(int i = 0; i < 4; i++)
	{
		if(SELECTOR_MODE[i] > 766){SELECTOR_MODE[i] = FIX_MODE;}
		else{
			if(SELECTOR_MODE[i] < 256){SELECTOR_MODE[i] = EXT_MODE;}
			else{SELECTOR_MODE[i] = GALET_MODE;}
		}
	}
	//! debug
	//if(SELECTOR_MODE[0] == GALET_MODE && SELECTOR_MODE[1] == FIX_MODE && SELECTOR_MODE[2] == EXT_MODE){debug_led_3_on();}

}
void get_selector_values(void);
void get_selector_values(void){
	// AI_SELECTOR_VALUE_1	(A3	ADC3/PC3)
	// AI_SELECTOR_VALUE_2	(A2	ADC2/PC2)
	// AI_SELECTOR_VALUE_3	(A1	ADC1/PC1)
	// AI_SELECTOR_VALUE_4	(A0	ADC0/PC0)
	
	float result = adc_read_10bit(ADC_MUX_ADC3, ADC_VREF_AVCC);
	SELECTOR_VALUE[0] = (result*5.0)/1023;
	result = adc_read_10bit(ADC_MUX_ADC2, ADC_VREF_AVCC);
	SELECTOR_VALUE[1] = (result*5.0)/1023;
	result = adc_read_10bit(ADC_MUX_ADC1, ADC_VREF_AVCC);
	SELECTOR_VALUE[2] = (result*5.0)/1023;
	result = adc_read_10bit(ADC_MUX_ADC0, ADC_VREF_AVCC);
	SELECTOR_VALUE[3] = (result*5.0)/1023;
	
	
	
}
void calculate_delays(void){
	for(int i = 0; i < 4; i++){
		switch(SELECTOR_MODE[i]){
			case FIX_MODE:
				DELAY[i] = 1;	// 1 delay for 5 seconds
			break;
			
			case GALET_MODE:
				//Selector voltage tresholds: 0.35 1.07 1.78 2.5 3.21 3.92 4.63
				// lenel 0V
				if(SELECTOR_VALUE[i] < 0.35){DELAY[i]								= 2;}	// 2 * 5ec = 10 sec
				// level 0.72V
				if(SELECTOR_VALUE[i] > 0.35 && SELECTOR_VALUE[i] < 1.07){DELAY[i]	= 4;}	// 4 * 5ec = 20 sec
				// level 1.43V
				if(SELECTOR_VALUE[i] > 1.07 && SELECTOR_VALUE[i] < 1.78){DELAY[i]	= 6;}	// 5ec = 30 sec
				// level 2.14V
				if(SELECTOR_VALUE[i] > 1.78 && SELECTOR_VALUE[i] < 2.5){DELAY[i]	= 12;}	// 12 * 5ec = 1 min _ 60 sec
				// level 2.86V
				if(SELECTOR_VALUE[i] > 2.5 && SELECTOR_VALUE[i] < 3.21){DELAY[i]	= 24;}	// 24 * 5ec =2 min _ 120 sec
				// level 3.57V
				if(SELECTOR_VALUE[i] > 3.21 && SELECTOR_VALUE[i] < 3.92){DELAY[i]	= 36;}	// 36 * 5ec =3 min _ 180 sec
				// level 4.28v
				if(SELECTOR_VALUE[i] > 3.92 && SELECTOR_VALUE[i] < 4.63){DELAY[i]	= 60;}	// 60 * 5ec =5 min _ 300 sec
				// level 5V
				if(SELECTOR_VALUE[i] > 4.63 ){DELAY[i]								= 120;}	// 120 * 5ec =10 min_ 600 sec
			break;
			
			default:
			break;
		}
	}
}


void end_timer(void);
void end_timer(void){
	gpio_set_pin_low(DO_END);
	STATUS_FLAG = END;
}
void emergency_stop(void);
void emergency_stop(void){
	gpio_set_pin_low(DO_END);
	gpio_set_pin_high(DO_CONTROL_1);
	gpio_set_pin_high(DO_CONTROL_2);
	gpio_set_pin_high(DO_CONTROL_3);
	gpio_set_pin_high(DO_CONTROL_4);
	STATUS_FLAG = EMERGENCY_STOP;
	while(1){;}
}
void turn_controls_off(void);
void turn_controls_off(void){
		gpio_set_pin_high(DO_CONTROL_1);
		gpio_set_pin_high(DO_CONTROL_2);
		gpio_set_pin_high(DO_CONTROL_3);
		gpio_set_pin_high(DO_CONTROL_4);
}

uint16_t RC_NETWORK_VOLTAGE;
void start_timer(void);
void start_timer(void){
	get_enabled_timers();
	get_selector_modes();
	get_selector_values();
	for(int i = 0; i <4; i++){
		if(TIMER_ENABLE_STATUS[i]){		// If timer[i] is enabled [equal to 0]
			do_control_clear(i);		// switch on the control output
			switch(SELECTOR_MODE[i]){	// Then check the counting mode of timer
				case FIX_MODE:			// If mode of timer is fixed time
					cpu_delay_ms(5000, F_CPU);//delay for 5 sec	// Hold the specified delay
				break;
				
				case GALET_MODE:		// If mode of timer is given from panel switch
					//cpu_delay_ms(10000, F_CPU);
					while(DELAY[i]>0){
						cpu_delay_ms(10000, F_CPU);
						DELAY[i] = DELAY[i] - 1;
					}
				// delay(delay)	// Hold the specified delay
				break;
				
				case EXT_MODE:			// If mode of timer is given from external RC-network
				RC_NETWORK_VOLTAGE = 0;
				gpio_set_pin_high(DO_EXT_START);
				while(RC_NETWORK_VOLTAGE < 511){	// while value from ADC do not exceed the threshold
					RC_NETWORK_VOLTAGE = get_rc_voltage(i);// read the voltage from RC-network in loop
				}
				gpio_set_pin_low(DO_EXT_START);
				break;
			}
		}
		do_control_set(i);
	}
}

// Implementation 1: All time intervals are formed by timer
int main(void){
	// Insert system clock initialization code here (sysclk_init()).
	board_init();
	interrupt_init();
	timer_init();

	//! UART for debug purposes
	//uart_init();

	// set prescaler and enable ADC
	adc_init(ADC_PRESCALER_DIV128);

	// Insert application code here, after the board has been initialized.
	sei();
	cpu_delay_ms(500, F_CPU);	// Service delay to avoid fails because of power-on noise
	FLAG = 0;					// Clear the status flag
	STATUS_FLAG = READY;		// Go to READY state
	while(1){
		switch(STATUS_FLAG){

			case READY:
			if(FLAG == START){
				FLAG = 0;
				gpio_set_pin_low(DO_END);				// Reset DO_END output
				cpu_delay_ms(20, F_CPU);				// This delay is required to pass LF-filter
																	
				if(ioport_get_pin_level(DI_BYPASS)==1){	// If DI_BYPASS input is set to 1
					get_enabled_timers();
/*
					TIMER_ENABLE_STATUS[0] = 1;
					TIMER_ENABLE_STATUS[1] = 1;
					TIMER_ENABLE_STATUS[2] = 1;
					TIMER_ENABLE_STATUS[3] = 0;
*/
					get_selector_modes();
/*
					SELECTOR_MODE[0] = FIX_MODE;
					SELECTOR_MODE[1] = GALET_MODE;
					SELECTOR_MODE[2] = EXT_MODE;
*/				
					get_selector_values();
/*
					SELECTOR_VALUE[1] = 0; //Selector voltage tresholds: 0.35 1.07 1.78 2.5 3.21 3.92 4.63
					SELECTOR_VALUE[2] = 0.6; 
*/					
					calculate_delays();
									
					CurrentTimer = 0;
					STATUS_FLAG = TIMER_SETUP;
				}
				else{
					STATUS_FLAG = END;
				}
				
			}
			break;
						
			case TIMER_SETUP:
			if(TIMER_ENABLE_STATUS[CurrentTimer]){				// If current timer is enabled
				do_control_clear(CurrentTimer);					// Set output of timer OFF(low)
				if(SELECTOR_MODE[CurrentTimer] == EXT_MODE){	// If external counter is used for this timer
					ioport_configure_pin(DO_EXT_START, IOPORT_DIR_OUTPUT |  IOPORT_INIT_LOW); // Change mode of EXT_START pin to output and set it's value to 0
					//gpio_set_pin_low(DO_EXT_START);			// Включаем внешний таймер
					cpu_delay_ms(100, F_CPU);				// Make 100 ms pulse duration 
					ioport_configure_pin(DO_EXT_START,		IOPORT_DIR_INPUT |  IOPORT_PULL_UP);// Change mode of EXT_START pin to input and set it's value to 1
					//gpio_set_pin_high(DO_EXT_START);			
					RC_STATUS = ON;					
				}
				else{										// If current timer mode is FIX or GALET
					timer_start(DELAY[CurrentTimer]);		// Start counting
				}
				STATUS_FLAG = TIMER_WORK;
			}
			else{											// If current timer is disabled
				if(CurrentTimer++ > 3){
					STATUS_FLAG = END;
				}
				
			}
			break;
			
			case TIMER_WORK:
			if(FLAG == FINISH){
				FLAG = 0;
				do_control_set(CurrentTimer);				// Set output of timer ON(high).
				if(CurrentTimer++ < 4){
					STATUS_FLAG = TIMER_SETUP;
				}
				else{
					STATUS_FLAG = END;
				}
			}
			else{
				if(SELECTOR_MODE[CurrentTimer] == EXT_MODE){
					
					if(ioport_get_pin_level(DI_RC_READY) == 0){	// If signal from external timer was received,
						FLAG = FINISH;							// turn FLAG into FINISH state
						RC_STATUS = OFF;
					}
				}
			}
			break;
			
			case END:
			gpio_set_pin_high(DO_END);		// Set up DO_END output
			STATUS_FLAG = READY;
			break;
			
			case STOP:
			turn_controls_off();
			gpio_set_pin_high(DO_END);		// Set up DO_END output
			cpu_delay_ms(20, F_CPU);		// Wait till STOP command will be processed by all previous timers
			FLAG = 0;						// And reset the flag, which could be set up by DO_END signal from previous timer
			STATUS_FLAG = READY;
			break;
		}
	}
}

// Implementation 2: All time intervals are formed by counting loops
/*int main (void)
{
	// Insert system clock initialization code here (sysclk_init()).
	board_init();
	interrupt_init();
	
	//! UART for debug purposes
	//uart_init();
	
	// set prescaler and enable ADC
	adc_init(ADC_PRESCALER_DIV128);
	
	// Insert application code here, after the board has been initialized.
	sei();
	STATUS_FLAG = BEGIN;
	//debug_led_3_on();

// 	while(1){
// 		for(int i = 0; i <4; i++){
// 			do_control_clear(i);		// switch on the control output
// 			cpu_delay_ms(5000, F_CPU);//delay for 5 sec	// Hold the specified delay
// 			do_control_set(i);
// 			cpu_delay_ms(100, F_CPU);//delay for 5 sec	// Hold the specified delay
// 		}
// 	}
	
	while(1){
		if(STATUS_FLAG == START){	// Waiting for START trigger
			gpio_set_pin_low(DO_END);
			cpu_delay_ms(10, F_CPU); // This delay is required to pass LF-filter
			if(ioport_get_pin_level(DI_BYPASS)==1){		// If BYPASS input is inactive
				start_timer();							// Start timers
				end_timer();
			}
			else{
				end_timer();
			}							// If BYPASS input is active - end of operation
		}
	}
}
*/

