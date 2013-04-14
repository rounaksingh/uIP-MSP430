msp430-uip_tcp-ip_stack
========================
This is written as tcp-ip stack for MSP430 series with ENC28J60 as NIC. This is 
just a modification to the uIP-avr tcp-ip stack, which uses uIP stack as base.
All credits to Adam Dunkels (adam@dunkels.com) & uIP-avr team (Vladimir S. Fonov 
(vladimir.fonov@gmail.com) & others). Also, credits to Jonathan Granade for 
writing ENC28J60 drivers.
This program can be complied with msp430-gcc.Programmed & debugged by mspdebug.

Overview:
=========
This program is tcp-ip stack for MSP430, which uses uIP-avr as base, which uses
uIP tcp-ip stack as base.
uIP-AVR is implementation of the TCP/IP protocol stack for AVR micro-controllers.
uIP is an implementation of the TCP/IP protocol stack intended for small 8-bit 
and 16-bit microcontrollers. uIP provides the necessary protocols for Internet 
communication, with a very small code footprint and RAM requirements - the uIP
code size is on the order of a few kilobytes and RAM usage is on the order of a 
few hundred bytes.
This program is written in c language. This is written for MSP430FG4816 Exp.
Board, but can be modified to any msp430 series.

Directory Structure:
====================
It contains three directories, which are net, net_app & serial_debug.

1> net/ --- This directory contains all the uIP source codes, which are required
for basic implementation of the tcp-ip stack. It also contains NIC (ie. ENC28J60)
drivers. These can be replaced as per required, depends on which NIC chip is being 
used.

2> net_app/ --- This directory contains the special net application, which is 
being implemented. For Example, if required to implement the web_client,
web_server, smtp protocol, etc, then there are different source files for that.
This directory source files can be modified as per need of user.
Actually, this file are provided by the origianl uIP stack.

3> serial_debug/ --- This directory contains the implementation of the debug 
interface by using UART of the msp430. Since, it is quite difficult for us to 
debug this big program just using LEDs or LCDs. So, this method is implemented
for convinence. The Serial Debug can be switch ON or OFF as needed by changing 
the value of the SERIAL_DEBUG macro in "uNetConfigure.h".

Implementation:
===============


