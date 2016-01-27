/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include <vars.h>
#include "adc.h"
#include <string.h>
#include "compiler.h"

extern uint8_t CurrentTimer;
extern uint32_t Timer_Delays;
extern uint16_t Timer_Int_Counter;

/**
 * \def BUFFER_SIZE
 * \brief The size of the UART buffer
 */
#define BUFFER_SIZE 20

// set the correct BAUD and F_CPU defines before including setbaud.h
#include "conf_clock.h"
#include "conf_uart.h"

/**
 * \name avr_libc_inc avr libc include files
 * @{
 */
#include <util/setbaud.h>
#include <avr/interrupt.h>

#include "ring_buffer.h"

volatile uint8_t STATUS_FLAG = 0;
volatile uint8_t RC_STATUS = 0;
volatile uint8_t FLAG = 0;


#define ADC_INPUT      ADC_MUX_ADC0
#define ADC_VREF       ADC_VREF_AVCC

// buffers for use with the ring buffer (belong to the UART)
uint8_t out_buffer[BUFFER_SIZE];
uint8_t in_buffer[BUFFER_SIZE];

// the string we send and receive on UART
const char test_string[] = "Hello, world!";

//! ring buffer to use for the UART transmission
struct ring_buffer ring_buffer_out;
//! ring buffer to use for the UART reception
struct ring_buffer ring_buffer_in;

/**
 * \brief UART data register empty interrupt handler
 *
 * This handler is called each time the UART data register is available for
 * sending data.
 */
ISR(UART0_DATA_EMPTY_IRQ)
{
	// if there is data in the ring buffer, fetch it and send it
	if (!ring_buffer_is_empty(&ring_buffer_out)) {
		UDR0 = ring_buffer_get(&ring_buffer_out);
	}
	else {
		// no more data to send, turn off data ready interrupt
		UCSR0B &= ~(1 << UDRIE0);
	}
}

/**
 * \brief Data RX interrupt handler
 *
 * This is the handler for UART receive data
 */
ISR(UART0_RX_IRQ)
{
	ring_buffer_put(&ring_buffer_in, UDR0);
}

/**
 * \brief Initialize the UART with correct baud rate settings
 *
 * This function will initialize the UART baud rate registers with the correct
 * values using the AVR libc setbaud utility. In addition set the UART to
 * 8-bit, 1 stop and no parity.
 */
static void uart_init(void)
{
#if defined UBRR0H
	// get the values from the setbaud tool
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
#else
#error "Device is not supported by the driver"
#endif

#if USE_2X
	UCSR0A |= (1 << U2X0);
#endif

	// enable RX and TX and set interrupts on rx complete
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

	// 8-bit, 1 stop bit, no parity, asynchronous UART
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00) | (0 << USBS0) |
			(0 << UPM01) | (0 << UPM00) | (0 << UMSEL01) |
			(0 << UMSEL00);

	// initialize the in and out buffer for the UART
	ring_buffer_out = ring_buffer_init(out_buffer, BUFFER_SIZE);
	ring_buffer_in = ring_buffer_init(in_buffer, BUFFER_SIZE);
}

/**
 * \brief Function for putting a char in the UART buffer
 *
 * \param data the data to add to the UART buffer and send
 *
 */
static inline void uart_putchar(uint8_t data)
{
	// Disable interrupts to get exclusive access to ring_buffer_out.
	cli();
	if (ring_buffer_is_empty(&ring_buffer_out)) {
		// First data in buffer, enable data ready interrupt
		UCSR0B |=  (1 << UDRIE0);
	}
	// Put data in buffer
	ring_buffer_put(&ring_buffer_out, data);

	// Re-enable interrupts
	sei();
}

/**
 * \brief Function for getting a char from the UART receive buffer
 *
 * \retval Next data byte in receive buffer
 */
static inline uint8_t uart_getchar(void)
{
	return ring_buffer_get(&ring_buffer_in);
}


/**
 * \brief Function to check if we have a char waiting in the UART receive buffer
 *
 * \retval true if data is waiting
 * \retval false if no data is waiting
 */
static inline bool uart_char_waiting(void)
{
	return !ring_buffer_is_empty(&ring_buffer_in);
}

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
#define RC_MODE		3

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
			if(SELECTOR_MODE[i] < 256){SELECTOR_MODE[i] = RC_MODE;}
			else{SELECTOR_MODE[i] = GALET_MODE;}
		}
	}
	//! debug
	//if(SELECTOR_MODE[0] == GALET_MODE && SELECTOR_MODE[1] == FIX_MODE && SELECTOR_MODE[2] == RC_MODE){debug_led_3_on();}

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
				DELAY[i] = 1;	// 1 задержка на 5 секунд
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
				
				case RC_MODE:			// If mode of timer is given from external RC-network
				RC_NETWORK_VOLTAGE = 0;
				gpio_set_pin_high(DO_RC_START);
				while(RC_NETWORK_VOLTAGE < 511){	// while value from ADC do not exceed the threshold
					RC_NETWORK_VOLTAGE = get_rc_voltage(i);// read the voltage from RC-network in loop
				}
				gpio_set_pin_low(DO_RC_START);
				break;
			}
		}
		do_control_set(i);
	}
}

// Вариант с реализацией задержки на таймере
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
	cpu_delay_ms(500, F_CPU);	// задержка при включении для защиты от помех и ложных срабатываний
	FLAG = 0;					// Сброс ложных флагов
	STATUS_FLAG = READY;		// Переход в состояние готов
	while(1){
		switch(STATUS_FLAG){

			case READY:
			if(FLAG == START){
				FLAG = 0;
				gpio_set_pin_low(DO_END);				// Сброс вывода КОНЕЦ										
				cpu_delay_ms(20, F_CPU);				// задержка необходима, чтобы пройти через фильтры			
																	
				if(ioport_get_pin_level(DI_BYPASS)==1){	// Если на входе байпас единица								
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
					SELECTOR_MODE[2] = RC_MODE;
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
			if(TIMER_ENABLE_STATUS[CurrentTimer]){			// Если текущий таймер разрешен
				do_control_clear(CurrentTimer);				// Сброс вывода таймера
				if(SELECTOR_MODE[CurrentTimer] == RC_MODE){	// Если времязадающей является внешняя RC-цепь
					ioport_configure_pin(DO_RC_START,		IOPORT_DIR_OUTPUT |  IOPORT_INIT_LOW); // Переводим вывод RC_START в режим выхода со значением 0
					//gpio_set_pin_low(DO_RC_START);			// Включаем внешний таймер
					cpu_delay_ms(100, F_CPU);				// Низким импульсом 100 мс
					ioport_configure_pin(DO_RC_START,		IOPORT_DIR_INPUT |  IOPORT_PULL_UP);// Переводим вывод RC_START в режим входа со значением 1
					//gpio_set_pin_high(DO_RC_START);			
					RC_STATUS = ON;					
				}
				else{										// Если время таймера фиксировано или определяется галетным переключателем
					timer_start(DELAY[CurrentTimer]);		// Запускаем таймер
				}
				STATUS_FLAG = TIMER_WORK;
			}
			else{											// Если текущий таймер не разрешен
				if(CurrentTimer++ > 3){
					STATUS_FLAG = END;
				}
				
			}
			break;
			
			case TIMER_WORK:
			if(FLAG == FINISH){
				FLAG = 0;
				do_control_set(CurrentTimer);				// Установка вывода таймера
				if(CurrentTimer++ < 4){
					STATUS_FLAG = TIMER_SETUP;
				}
				else{
					STATUS_FLAG = END;
				}
			}
			else{
				if(SELECTOR_MODE[CurrentTimer] == RC_MODE){
					
					if(ioport_get_pin_level(DI_RC_READY) == 0){	// Если пришел сигнал с внешнего таймера
						FLAG = FINISH;							// Устанавливаем флаг FINISH
						RC_STATUS = OFF;
					}
				}
			}
			break;
			
			case END:
			gpio_set_pin_high(DO_END);		// Установка вывода КОНЕЦ
			STATUS_FLAG = READY;
			break;
			
			case STOP:
			turn_controls_off();
			gpio_set_pin_high(DO_END);		// Установка вывода КОНЕЦ
			cpu_delay_ms(20, F_CPU);		// Ожидаем, пока команду STOP отработают все предыдущие таймеры
			FLAG = 0;						// И сбрасываем флаг, который мог установиться выходом КОНЕЦ предыдущего таймера
			STATUS_FLAG = READY;
			break;
		}
	}
}

//Вариант без таймера (задержки реализуются простым остчетом в цикле)
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
			cpu_delay_ms(10, F_CPU); // необходимо, чтобы пройти через фильтры
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

