/*
 * control_output.h
 *
 * Created: 05.10.2015 22:17:04
 *  Author: prokopiy
 */ 


#ifndef CONTROL_OUTPUT_H_
#define CONTROL_OUTPUT_H_

#include "gpio.h"

#define Output_Off(control_outut)     gpio_set_pin_high(control_outut)
#define Output_On(control_outut)      gpio_set_pin_low(control_outut)




#endif /* CONTROL_OUTPUT_H_ */