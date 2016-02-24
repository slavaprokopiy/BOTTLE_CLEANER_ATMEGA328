 
 // I/O setup
 
 // Inputs discrete_______________________________________________________________
 // DI_ AVARIYA (External interrupt PCINT17)
 
 // DI_START	(External interrupt PCINT16)
 
 // DI_BYPASS	(PD2)
 DDRD &= ~(1 << PORTD0);
 // DI_TIMER_ENABLE_1	(13	PB5)
 DDRB &= ~(1 << PORTB5);
 // DI_TIMER_ENABLE_2	(12	PB4)
 DDRB &= ~(1 << PORTB4);
 // DI_TIMER_ENABLE_3	(11	PB3)
 DDRB &= ~(1 << PORTB3);
 // DI_TIMER_ENABLE_4	(10	PB2)
 DDRB &= ~(1 << PORTB2);
 
 // Inputs analog_________________________________________________________________
  // AI_SELECTOR_TYPE_1	(A5	ADC5/PC5)
 // AI_SELECTOR_TYPE_2	(A4	ADC4/PC4)
 // AI_SELECTOR_TYPE_3	(A7	ADC7)
 // AI_SELECTOR_TYPE_4	(A6	ADC6)
 
 // AI_SELECTOR_VALUE_1	(A3	ADC3/PC3)
 // AI_SELECTOR_VALUE_2	(A2	ADC2/PC2)
 // AI_SELECTOR_VALUE_3	(A1	ADC1/PC1)
 // AI_SELECTOR_VALUE_4	(A0	ADC0/PC0)
 
 // Outputs discrete_______________________________________________________________
 // DO_EXT_RC_START	(PD4)
 DDRD |= (1 << PORTD4);
 // DO_END			(PD3)
 DDRD |= (1 << PORTD3);
 // DO_CONTROL_1	(PD6)
 DDRD |= (1 << PORTD6);
 // DO_CONTROL_2	(PD7)
 DDRD |= (1 << PORTD7);
 // DO_CONTROL_3	(PB0)
 DDRB |= (1 << PORTB0);
 // DO_CONTROL_4	(PB1)
 DDRB |= (1 << PORTB1);
}