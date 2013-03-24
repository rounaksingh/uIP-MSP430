#include "msp430xG46x.h"
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