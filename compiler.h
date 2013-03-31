#ifndef __COMPILER_H__
#define __COMPILER_H__

// choose your AVR device here
//#include <iom128v.h>
#include <msp430.h>
#include <legacymsp430.h>
#define outp(val, reg)  (reg = val)
#define inp(reg)        (reg)

#define SIGNAL(x)       void x(void)  

#define cbi(p, q) ((p) &= ~_BV(q))
#define sbi(p, q) ((p) |= _BV(q))


typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

#endif /* __COMPILER_H__ */

