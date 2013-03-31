/****************************************************************
 *
 *
 *
 *
 *
*****************************************************************/

#ifndef __SERIAL_DEBUG_H__
#define __SERIAL_DEBUG_H__

#include <stdio.h>

//Forward Declarations aka Function Prototypes
void debug_putc(unsigned char c);
void SendBuffer(unsigned char* buf,unsigned short length);
void debug_serial_init();
void debug_puts(char* str);
/* PACKET_TERMINATION_CHAR is the byte whos value signals that the packet has finished transmitting.
* This example is built for processing strings so that character is the newline constant */
#define PACKET_TERMINATION_CHAR '\n'


#endif
