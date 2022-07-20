#pragma once

void timer_init(void);
void timer_irq_handler(void);
/**
 * Cinfiguring Timer
 * 
 * Comaprator Register (CVAL) = 64-bit register
 * Timer Register (TVAL)
 * 
 * Timer triggers an interrupt to CPU core when clock count (sysmte counter) read the CVAL configured value.
 * 
 * Processor reads system counter and compare to CVAL
 * 
 * Timer register is 32-bit register. 
 * SW writes this register, the processor reads the current system count internally, add the
 * written values, and then popluates CVAL. The value in timer register decides how long it takes
 * before time interrupt is triggered
 * 
 * System counter increment a rate defined by frequency register
 * 
 * 
 * Control register = Generic Timer Control Register
 * 1. enable timer interrupt   bit 0 is used to enable, bit 1 is timer interrupt mask bit
 * 2, report timer interrupt   bit 2 is used to see if current interrupt is timer
 * 
 * 
 * 
 * 
 * 
 */