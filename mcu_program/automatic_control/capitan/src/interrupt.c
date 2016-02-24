/*
 * interrupt.c
 *
 * Created: 09.10.2015 17:13:23
 *  Author: prokopiy
 */ 
#include <vars.h>
#include <asf.h>
extern uint8_t STATUS_FLAG;
extern uint8_t FLAG;

void debug_led_1_on(void);
void debug_led_2_on(void);
void debug_led_3_on(void);
void debug_led_1_off(void);
void debug_led_2_off(void);
void debug_led_3_off(void);

void emergency_stop(void);

void interrupt_init(void);

void interrupt_init(void)
{
	// enable INT0 DI_AVARIYA	External interrupt  (INT0/PD2)
	// on rising edge
	EIMSK |= (1<<INT0);
	EICRA |= (1<<ISC00)|(1<<ISC01);
	
	// enable INT1 DI_START		External interrupt (INT1/PD3)
	// on rising edge
	EIMSK |= (1<<INT1);
	EICRA |= (1<<ISC10)|(1<<ISC11);
	
}
ISR(INT0_vect)	// INT0 DI_AVARIYA	External interrupt  (INT0/PD2)
{
	// Для варианта программы без использования таймера
	/*//debug_led_3_off();
	emergency_stop();*/
	STATUS_FLAG = STOP;
}

ISR(INT1_vect)// INT1 DI_START		External interrupt (INT1/PD3)
{
	// Для варианта программы без использования таймера
	/*if(STATUS_FLAG == BEGIN || STATUS_FLAG == END){
		//debug_led_3_off();
		STATUS_FLAG = START;
	}*/
	
	//Для варианта программы с использованием таймера
	FLAG = START;
	
}