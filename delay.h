#ifndef DELAY_H
#define DELAY_H

#include <stdint.h>


#ifndef F_CPU
#define F_CPU 7995392
#endif

void _delay_ms(uint32_t);
void _delay_us(uint32_t);




#endif