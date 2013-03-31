/****************************************************************************
 *
 *
 *
****************************************************************************/

#include <msp430.h>
#include "serial_debug.h"
#include <string.h>

//---------------------------------------------------------------------------------------------------------------

void debug_putc(unsigned char);
void debug_puts(char *);
//------------------------------------------------------------------------------
/*
* This function sets up the USCI A0 in UART mode and sets the internal multiplexer to use P2.4 & P2.5
* and not P4.6 & P4.7. This is done because the DB 9 connector on the MSP430FG4618/F2013 Experimenter's
* Board is connected to those pins which are not the default pins used by the USCI. Note: This function
* is set to use a 115200 bit rate if you want to use a different bitrate use the tool at the following
* website: http://www.daycounter.com/Calculators/MSP430-Uart-Calculator.phtml
*/
void debug_serial_init()
{
   UCA0CTL1 |= UCSWRST;      //Configure the USCI with the reset bit held high
   P4SEL &= ~0x0C0;          //P4.7,6 != USCI_A0 RXD/TXD
   P2SEL |= 0x30;            //Set the USCI to use P2.4 and P2.5 for RX/TX
   UCA0CTL1 |= UCSSEL_2;     //Use SMCLK
   UCA0BR0 = 0x45;           //Set Bit Rate to 115200bps with a 8Mhz clock
   UCA0BR1 = 0x00;
   UCA0MCTL = 0x4A;          //Modulation
   UCA0CTL1 &= ~UCSWRST;     //Done configuring so take USCI out of reset
  
return;
}

/*
* The following function uses the USCI A0 setup in UART mode to transmit a single byte over RS-232
*/
void debug_putc(unsigned char c)
{
   while(!(IFG2 & UCA0TXIFG));   //Wait until transmit buffer is empty
   UCA0TXBUF = c;                 //Send character
	return;
}
//---------------------------------------------------------------------------------------------------------------

/*
* The following function sends a null terminated c string over RS-232 using the USCI A0 in UART mode
*/
void debug_puts(char* str)
{
   unsigned short i;
   for(i=0;str[i] != '\0';i++) debug_putc(str[i]); //Send string one character at a time
return;
}
//---------------------------------------------------------------------------------------------------------------

/*
* The following function sends an array of bytes of a given length over RS-232 using the USCI A0 in UART mode
*/
void SendBuffer(unsigned char* buf,unsigned short length)
{
   unsigned short i;
   for(i=0;i<length;i++) debug_putc(buf[i]); //Send buffer one character at a time
return;
}
//---------------------------------------------------------------------------------------------------------------

/*
* The following function is defined for the working of the built-in printf function, it is declared in stdio.h
*/

int puts(const char *s)
{
	debug_puts(s);
	return strlen(s);
}

//----------------------------------------------------------------------------------------------------------------

/*
* The following function is defined for the working of the built-in printf function, it is declared in stdio.h
*/

int putchar(int c)
{
	debug_putc(c);
	return 1;
}

//----------------------------------------------------------------------------------------------------------------
