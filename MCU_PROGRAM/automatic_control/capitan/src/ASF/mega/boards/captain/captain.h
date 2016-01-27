/*
 * IncFile1.h
 *
 * Created: 05.10.2015 21:27:15
 *  Author: prokopiy
 */ 


#ifndef CAPTAIN_H_
#define CAPTAIN_H_

#include "compiler.h"

# include "control_output.h"

#define MCU_SOC_NAME        "ATMEGA328P"
#define BOARD_NAME          "CAPTAIN"

// Inputs interrupt______________________________________________________________
// DI_AVARIYA	External interrupt  (INT0/PD2)
// DI_START		External interrupt (INT1/PD3)

// Inputs discrete_______________________________________________________________

// DI_BYPASS	(PD0)
#define DI_BYPASS_GPIO			IOPORT_CREATE_PIN(PORTD, 0)
#define DI_BYPASS				DI_BYPASS_GPIO

// DI_RC_READY	(PD5)
#define DI_RC_READY_GPIO		IOPORT_CREATE_PIN(PORTD, 5)
#define DI_RC_READY				DI_RC_READY_GPIO

// DI_TIMER_ENABLE_1	(13	PB2)
#define DI_TIMER_ENABLE_1_GPIO	IOPORT_CREATE_PIN(PORTB, 2)
#define DI_TIMER_ENABLE_1		DI_TIMER_ENABLE_1_GPIO

// DI_TIMER_ENABLE_2	(12	PB3)
#define DI_TIMER_ENABLE_2_GPIO	IOPORT_CREATE_PIN(PORTB, 3)
#define DI_TIMER_ENABLE_2		DI_TIMER_ENABLE_2_GPIO

// DI_TIMER_ENABLE_3	(11	PB4)
#define DI_TIMER_ENABLE_3_GPIO	IOPORT_CREATE_PIN(PORTB, 4)
#define DI_TIMER_ENABLE_3		DI_TIMER_ENABLE_3_GPIO

// DI_TIMER_ENABLE_4	(10	PB5)
#define DI_TIMER_ENABLE_4_GPIO	IOPORT_CREATE_PIN(PORTB, 5)
#define DI_TIMER_ENABLE_4		DI_TIMER_ENABLE_4_GPIO

// Outputs discrete_______________________________________________________________
// DO_EXT_START	(PD4)
#define DO_EXT_START_GPIO	IOPORT_CREATE_PIN(PORTD, 4)
#define DO_EXT_START			DO_EXT_START_GPIO

// DO_END			(PD1)
#define DO_END_GPIO				IOPORT_CREATE_PIN(PORTD, 1)
#define DO_END					DO_END_GPIO

// DO_CONTROL_1	(PD6)
#define DO_CONTROL_1_GPIO		IOPORT_CREATE_PIN(PORTD, 6)
#define DO_CONTROL_1			DO_CONTROL_1_GPIO

// DO_CONTROL_2	(PD7)
#define DO_CONTROL_2_GPIO		IOPORT_CREATE_PIN(PORTD, 7)
#define DO_CONTROL_2			DO_CONTROL_2_GPIO

// DO_CONTROL_3	(PB0)
#define DO_CONTROL_3_GPIO		IOPORT_CREATE_PIN(PORTB, 0)
#define DO_CONTROL_3			DO_CONTROL_3_GPIO

// DO_CONTROL_4	(PB1)
#define DO_CONTROL_4_GPIO		IOPORT_CREATE_PIN(PORTB, 1)
#define DO_CONTROL_4			DO_CONTROL_4_GPIO

// DEBUG_LED_1
#define DEBUG_LED_1_GPIO		IOPORT_CREATE_PIN(PORTC, 4)
#define DEBUG_LED_1				DEBUG_LED_1_GPIO

// DEBUG_LED_2
#define DEBUG_LED_2_GPIO		IOPORT_CREATE_PIN(PORTC, 5)
#define DEBUG_LED_2				DEBUG_LED_2_GPIO 

// DEBUG_LED_3
#define DEBUG_LED_3_GPIO		IOPORT_CREATE_PIN(PORTB, 5)
#define DEBUG_LED_3				DEBUG_LED_3_GPIO
 

 // Inputs analog_________________________________________________________________
 // AI_SELECTOR_TYPE_1	(A4	ADC4/PC4)
 #define ADC_4_GPIO		IOPORT_CREATE_PIN(PORTC, 4)
 #define ADC_4			ADC_4_GPIO
 // AI_SELECTOR_TYPE_2	(A5	ADC5/PC5)
 #define ADC_5_GPIO		IOPORT_CREATE_PIN(PORTC, 5)
 #define ADC_5			ADC_5_GPIO
 // AI_SELECTOR_TYPE_3	(A6	ADC6)
 // AI_SELECTOR_TYPE_4	(A7	ADC7)
  
 // AI_SELECTOR_VALUE_1	(A3	ADC3/PC3)
 #define ADC_3_GPIO		IOPORT_CREATE_PIN(PORTC, 3)
 #define ADC_3			ADC_3_GPIO
 // AI_SELECTOR_VALUE_2	(A2	ADC2/PC2)
 #define ADC_2_GPIO		IOPORT_CREATE_PIN(PORTC, 2)
 #define ADC_2			ADC_2_GPIO
 // AI_SELECTOR_VALUE_3	(A1	ADC1/PC1)
  #define ADC_1_GPIO	IOPORT_CREATE_PIN(PORTC, 1)
  #define ADC_1			ADC_1_GPIO
 // AI_SELECTOR_VALUE_4	(A0	ADC0/PC0)
#define ADC_0_GPIO		IOPORT_CREATE_PIN(PORTC, 0)
#define ADC_0			ADC_0_GPIO

#endif /* INCFILE1_H_ */