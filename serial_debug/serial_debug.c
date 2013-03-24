/****************************************************************************
 *
 *
 *
****************************************************************************/

#include  "msp430xG46x.h"
#include "serial_debug.h"
#include "stdarg.h"

//---------------------------------------------------------------------------------------------------------------

void putc(unsigned char);
void puts(char *);

static const unsigned long dv[] = {
//  4294967296      // 32 bit unsigned max
    1000000000,     // +0
     100000000,     // +1
      10000000,     // +2
       1000000,     // +3
        100000,     // +4
//       65535      // 16 bit unsigned max     
         10000,     // +5
          1000,     // +6
           100,     // +7
            10,     // +8
             1,     // +9
};

static void xtoa(unsigned long x, const unsigned long *dp)
{
    char c;
    unsigned long d;
    if(x) {
        while(x < *dp) ++dp;
        do {
            d = *dp++;
            c = '0';
            while(x >= d) ++c, x -= d;
            putc(c);
        } while(!(d & 1));
    } else
        putc('0');
}

static void puth(unsigned n)
{
    static const char hex[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    putc(hex[n & 15]);
}
 
void printf(char *format, ...)
{
    char c;
    int i;
    long n;
    
    va_list a;
    va_start(a, format);
    while(c = *format++) {
        if(c == '%') {
            switch(c = *format++) {
                case 's':                       // String
                    puts(va_arg(a, char*));
                    break;
                case 'c':                       // Char
                    putc(va_arg(a, char));
                    break;
                case 'i':                       // 16 bit Integer
                case 'u':                       // 16 bit Unsigned
                    i = va_arg(a, int);
                    if(c == 'i' && i < 0) i = -i, putc('-');
                    xtoa((unsigned)i, dv + 5);
                    break;
                case 'l':                       // 32 bit Long
                case 'n':                       // 32 bit uNsigned loNg
                    n = va_arg(a, long);
                    if(c == 'l' &&  n < 0) n = -n, putc('-');
                    xtoa((unsigned long)n, dv);
                    break;
                case 'x':                       // 16 bit heXadecimal
                    i = va_arg(a, int);
                    puth(i >> 12);
                    puth(i >> 8);
                    puth(i >> 4);
                    puth(i);
                    break;
                case 0: return;
                default: goto bad_fmt;
            }
        } else
bad_fmt:    putc(c);
    }
    va_end(a);
}

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
void putc(unsigned char c)
{
   while(!(IFG2 & UCA0TXIFG));   //Wait until transmit buffer is empty
   UCA0TXBUF = c;                 //Send character
return;
}
//---------------------------------------------------------------------------------------------------------------

/*
* The following function sends a null terminated c string over RS-232 using the USCI A0 in UART mode
*/
void puts(char* str)
{
   unsigned short i;
   for(i=0;str[i] != '\0';i++) putc(str[i]); //Send string one character at a time
return;
}
//---------------------------------------------------------------------------------------------------------------

/*
* The following function sends an array of bytes of a given length over RS-232 using the USCI A0 in UART mode
*/
void SendBuffer(unsigned char* buf,unsigned short length)
{
   unsigned short i;
   for(i=0;i<length;i++) putc(buf[i]); //Send buffer one character at a time
return;
}
//---------------------------------------------------------------------------------------------------------------


