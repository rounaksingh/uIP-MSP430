//  Vladimir S. Fonov ~ vladimir.fonov <at> gmail.com
#include "uNetConfigure.h"
#include "delay.h"
#include "compiler.h"

#if DEBUG_SERIAL
//#include "SerialStream.h" // UART printf
#endif //DEBUG_SERIAL

#include "net/clock.h"
#include "net/timer.h"
#include "net/uip.h"
#include "net/nic.h"
#include "net/uip_arp.h"
#include "net_app/dhcpc.h"

#include "net_app/simple-app.h"

#if UIP_SPLIT_HACK
#include "net/uip-split.h"
#elif UIP_EMPTY_PACKET_HACK
#include "net/uip-empty-packet.h"
#endif 

#define BUF ((struct uip_eth_hdr *)&uip_buf[0])

#ifndef TASK
//replacement for LUFA tasks
#define TASK(fun) void fun(void)
#endif //TASK

TASK(NetTask);

struct timer periodic_timer, arp_timer;
static clock_time_t timerCounter=0;

static struct uip_eth_addr uNet_eth_address;

// TCP/IP parameters in data memory
uint8_t _enable_dhcp=USE_DHCP;
uint8_t _eth_addr[6]={UIP_ETHADDR0,UIP_ETHADDR1,UIP_ETHADDR2,UIP_ETHADDR3,UIP_ETHADDR4,UIP_ETHADDR5};
uint8_t _ip_addr[4]={UIP_IPADDR0, UIP_IPADDR1, UIP_IPADDR2, UIP_IPADDR3};
uint8_t _net_mask[4]={UIP_NETMASK0, UIP_NETMASK1, UIP_NETMASK2, UIP_NETMASK3};
uint8_t _gateway[4]={UIP_DRIPADDR0, UIP_DRIPADDR1, UIP_DRIPADDR2, UIP_DRIPADDR3};

clock_time_t clock_time(void)
{
  return timerCounter;
}

  /*
*  This function sets up Timer A to use ACLK which is a 32.768kHz Watch Crystal, to run in "Up Mode" and generate
*  an interrupt every second
*/
//! initialize clock
void clock_init(void)
{
   TACTL = (TASSEL_1 | TACLR);            //Use ACLK 32.768Khz, and reset TAR
   TACCTL0 = CCIE;                        //Enable the Capture/Compare Interrupt
   TACCR0 = 32767;                        //The counter will reach 32767 in approx. 1 second
   TACTL |= MC_1;                         //Set it to run in 'up mode' and thus start the counter
}

//---------------------------------------------------------------------------------------------------------------

/*
* This function is the interrupt service routine for Timer A. The timer is setup
* to generate a interrupt and run the contents of this function every second.
*/
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
  timerCounter++;
  P5OUT|=BIT1;
  _delay_ms(100);
  P5OUT&=~BIT1;
}

//! log uip messages
void uip_log(char *msg)
{
#if DEBUG_SERIAL
  //printf(msg);
#endif 
}

// Callback for when DHCP client has been configured.
void dhcpc_configured(const struct dhcpc_state *s)
{
	uip_sethostaddr(&s->ipaddr);
	uip_setnetmask(&s->netmask);
	uip_setdraddr(&s->default_router);
}

int main(void)
{
	uint8_t i;
        
  P5DIR|=BIT1;
  //P5OUT&=~BIT1;
  P5OUT|=BIT1;
  _DINT();                                //Disable Interrupts   -- For initlization purposes
   WDTCTL = WDTPW + WDTHOLD;               //Stop WDT

   /*
   Set System Clock Frequency to (121+1) * 32786 * 2 = 7.995392Mhz
   This freq. allows for 2400 - 230400 bit rate UART operation with less than 0.86% error
   */
   FLL_CTL0 = (DCOPLUS | XCAP18PF);       //Enable Frequency Multiplier (DCO+) and XIN/XOUT caps to 18pF
   SCFI0 = (FLLD_2 | FN_8);               //Set Multiplier to 2, and DCO range to 8MHz nominal
   SCFQCTL = 121;                         //7.995392Mhz Operation with No Modulation
  
  //setup serial port 
  
#if DEBUG_SERIAL
  //SerialStream_Init(9600,0);
#endif 
	
	for(i=0;i<6;i++)
		uNet_eth_address.addr[i]=_eth_addr[i];
	
  // init NIC device driver
#if DEBUG_SERIAL
 	/*
	printf_P(PSTR("MAC address:%02x:%02x:%02x:%02x:%02x:%02x\r\n"),
			(int)_eth_addr[0],(int)_eth_addr[1],(int)_eth_addr[2],
			(int)_eth_addr[3],(int)_eth_addr[4],(int)_eth_addr[5]);

  printf_P(PSTR("Initializing NIC...\r\n"));
  */
#endif

  nic_init(_eth_addr);

  uip_ipaddr_t ipaddr;

	uip_setethaddr(uNet_eth_address);

#if DEBUG_SERIAL
  //printf_P(PSTR("Initializing uIP...\r\n"));
#endif 
  //init uIP
  uip_init();

#if DEBUG_SERIAL
  //printf_P(PSTR("Initializing ARP...\r\n"));
#endif 
  //init ARP cache
  uip_arp_init();

#if DEBUG_SERIAL
  //printf_P(PSTR("Initializing timer...\r\n"));
#endif 
  // init periodic timer
  clock_init();

  _EINT();                               // reenable interrupts


	if(!_enable_dhcp)
	{

#if DEBUG_SERIAL
  	//printf_P(PSTR("Initializing tcp/ip settings\r\n"));
#endif
	
#if DEBUG_SERIAL
/*
  	printf_P(PSTR("IP %d.%d.%d.%d\r\n"),
			(int)_ip_addr[0],(int)_ip_addr[1],(int)_ip_addr[2],(int)_ip_addr[3]);

  	printf_P(PSTR("NetMask %d.%d.%d.%d\r\n"),
			(int)_net_mask[0],(int)_net_mask[1],(int)_net_mask[2],(int)_net_mask[3]);

  	printf_P(PSTR("Gateway %d.%d.%d.%d\r\n"),
			(int)_gateway[0],(int)_gateway[1],(int)_gateway[2],(int)_gateway[3]);
*/
#endif

		uip_ipaddr(ipaddr, _ip_addr[0], _ip_addr[1], _ip_addr[2], _ip_addr[3]);
    uip_sethostaddr(ipaddr);

    uip_ipaddr(ipaddr, _net_mask[0], _net_mask[1], _net_mask[2], _net_mask[3]);
    uip_setnetmask(ipaddr);

		uip_ipaddr(ipaddr, _gateway[0], _gateway[1], _gateway[2], _gateway[3]);
		uip_setdraddr(ipaddr);
	} else {
#if DEBUG_SERIAL
	//	printf_P(PSTR("Going to query DHCP server...\r\n"));
#endif

    uip_ipaddr(ipaddr, 0, 0, 0, 0);
    uip_sethostaddr(ipaddr);
    uip_setnetmask(ipaddr);
		uip_setdraddr(ipaddr);
	}

  timer_set(&periodic_timer, CLOCK_SECOND * 2 );
  timer_set(&arp_timer, CLOCK_SECOND * 10);

	example1_init();

	if(_enable_dhcp)
	{
		dhcpc_init(&uNet_eth_address.addr[0], 6);
		dhcpc_request();
	}

	/* Initialize Scheduler so that it can be used */
	//Scheduler_Init();

	/* Initialize USB Subsystem */
	//USB_Init();

	//Scheduler_SetTaskMode(NetTask, TASK_RUN);


	/* Scheduling - routine never returns, so put this last in the main function */
	//Scheduler_Start();
#if DEBUG_SERIAL
		//printf_P(PSTR("Starting main loop...\r\n"));
#endif
	while(1) {
		NetTask();
	}
}

//! netork loop
TASK(NetTask)
{
  u8_t i;

	uip_len = nic_poll();
	if(uip_len > 0) {

		if(BUF->type == htons(UIP_ETHTYPE_IP)) {
			
			uip_arp_ipin();
			uip_input();

			/* If the above function invocation resulted in data that
					should be sent out on the network, the global variable
					uip_len is set to a value > 0. */
			if(uip_len > 0) {
				uip_arp_out();
#if UIP_SPLIT_HACK
				uip_split_output();
#elif UIP_EMPTY_PACKET_HACK
				uip_emtpy_packet_output();
#else 
				nic_send();
#endif 
			}
		} else if(BUF->type == htons(UIP_ETHTYPE_ARP)) {

			uip_arp_arpin();
			/* If the above function invocation resulted in data that
					should be sent out on the network, the global variable
					uip_len is set to a value > 0. */
			if(uip_len > 0) {

				nic_send();

			}
		}

	} else if(timer_expired(&periodic_timer)) {
		timer_reset(&periodic_timer);
		for(i = 0; i < UIP_CONNS; i++) {
			uip_periodic(i);
			/* If the above function invocation resulted in data that
					should be sent out on the network, the global variable
					uip_len is set to a value > 0. */
			if(uip_len > 0) {
				uip_arp_out();
#if UIP_SPLIT_HACK
				uip_split_output();
#elif UIP_EMPTY_PACKET_HACK
				uip_emtpy_packet_output();
#else 
				nic_send();
#endif 
			}
		}

#if UIP_UDP
		for(i = 0; i < UIP_UDP_CONNS; i++) {
			uip_udp_periodic(i);
			/* If the above function invocation resulted in data that
					should be sent out on the network, the global variable
					uip_len is set to a value > 0. */
			if(uip_len > 0) {
				uip_arp_out();
				nic_send();
			}
		}
#endif /* UIP_UDP */
		
		/* Call the ARP timer function every 10 seconds. */
		if(timer_expired(&arp_timer)) {
			timer_reset(&arp_timer);
			uip_arp_timer();
		}
	}
	while(nic_sending()) {asm("nop");}; //wait untill packet is sent away
}


