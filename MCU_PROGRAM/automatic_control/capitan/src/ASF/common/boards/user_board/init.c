/**
 * \file
 *
 * \brief User board initialization template
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include <compiler.h>
#include <board.h>
#include <conf_board.h>
#include "gpio.h"
#include "control_output.h"

void board_init(void)
{
		/* Analog inputs initialization */
		//
		/* Discrete inputs initialization */
		ioport_configure_pin(DI_BYPASS,			IOPORT_DIR_INPUT |  IOPORT_PULL_DOWN);
		ioport_configure_pin(DI_RC_READY,		IOPORT_DIR_INPUT |  IOPORT_PULL_DOWN);
		ioport_configure_pin(DI_TIMER_ENABLE_1,	IOPORT_DIR_INPUT |  IOPORT_PULL_DOWN);
		ioport_configure_pin(DI_TIMER_ENABLE_2,	IOPORT_DIR_INPUT |  IOPORT_PULL_DOWN);
		ioport_configure_pin(DI_TIMER_ENABLE_3,	IOPORT_DIR_INPUT |  IOPORT_PULL_DOWN);
		ioport_configure_pin(DI_TIMER_ENABLE_4,	IOPORT_DIR_INPUT |  IOPORT_PULL_DOWN);
		
		/* Discrete outputs initialization */
		ioport_configure_pin(DO_RC_START,		IOPORT_DIR_INPUT |  IOPORT_PULL_UP);
		ioport_configure_pin(DO_END,			IOPORT_DIR_OUTPUT |  IOPORT_INIT_HIGH);
		ioport_configure_pin(DO_CONTROL_1,		IOPORT_DIR_OUTPUT |  IOPORT_INIT_HIGH);
		ioport_configure_pin(DO_CONTROL_2,		IOPORT_DIR_OUTPUT |  IOPORT_INIT_HIGH);
		ioport_configure_pin(DO_CONTROL_3,		IOPORT_DIR_OUTPUT |  IOPORT_INIT_HIGH);
		ioport_configure_pin(DO_CONTROL_4,		IOPORT_DIR_OUTPUT |  IOPORT_INIT_HIGH);
		
		ioport_configure_pin(DEBUG_LED_1,		IOPORT_DIR_OUTPUT |  IOPORT_INIT_LOW);
		ioport_configure_pin(DEBUG_LED_2,		IOPORT_DIR_OUTPUT |  IOPORT_INIT_LOW);
		ioport_configure_pin(DEBUG_LED_3,		IOPORT_DIR_OUTPUT |  IOPORT_INIT_LOW);
		
		ioport_configure_pin(ADC_0,		IOPORT_DIR_INPUT);
		ioport_configure_pin(ADC_1,		IOPORT_DIR_INPUT);
		ioport_configure_pin(ADC_2,		IOPORT_DIR_INPUT);
		ioport_configure_pin(ADC_3,		IOPORT_DIR_INPUT);
		ioport_configure_pin(ADC_4,		IOPORT_DIR_INPUT);
		ioport_configure_pin(ADC_5,		IOPORT_DIR_INPUT);
}
