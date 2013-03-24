#ifndef __COMPILER_H__
#define __COMPILER_H__

// choose your AVR device here
//#include <iom128v.h>
#include "msp430xG46x.h"

#define outp(val, reg)  (reg = val)
#define inp(reg)        (reg)

#define cli()           CLI()
#define sei()           SEI()
//#define cbi(reg, bit)   (reg &= ~(1<<bit))
//#define sbi(reg, bit)   (reg |= (1<<bit))

#define SIGNAL(x)       void x(void)  

#define nop() NOP()

#define _BV(x)	   (1<<x)


//#include <avr/signal.h>
//#include <avr/interrupt.h>
//#include <avr/io.h>

#define outp(val, reg)  (reg = val)
#define inp(reg)        (reg)

#define cbi(p, q) ((p) &= ~_BV(q))
#define sbi(p, q) ((p) |= _BV(q))

//#define nop() asm volatile("nop\n\t"::);


typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

#endif /* __COMPILER_H__ */

