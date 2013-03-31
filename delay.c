#include <msp430.h>
#include "delay.h"

void _delay_ms(uint32_t cycles)
{
 while((cycles--))
 __delay_cycles(F_CPU/1000); 
  
}

void _delay_us(uint32_t cycles)
{
while((cycles--))
 __delay_cycles(F_CPU/1000000); 
   
}

static void __inline__ brief_pause(register unsigned int n)
{
    __asm__ __volatile__ (
		"1: \n"
		" dec	%[n] \n"
		" jne	1b \n"
        : [n] "+r"(n));
}

