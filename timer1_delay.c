/**************************************************   *
 * precise delay of 1 ms in PIC18F4520 using Timer1 
 * Author: suraj shinde                           *              *
 **************************************************/
#pragma config OSC=INTIO67, LVP=OFF, WDT=OFF, PBADEN=OFF

#include <pic18f4520.h>		/* Contains PIC18F4550 specifications */

#define Pulse LATB  		/* Define Pulse as LATB to output on PORTB */

void Timer1_delay();

void main()
{
    //OSCCON=0x72;  		/* Configure oscillator frequency to 8MHz */
    OSCCONbits.IRCF0=1;            //set the oscillator to 8MHz
    OSCCONbits.IRCF1=1;
    OSCCONbits.IRCF2=1;
    TRISB=0;  			/* Set as output port */
    Pulse=0xff;  		/* Send high on PortB */
    T1CON=0x80;
    
    while(1)
    {
        Pulse=~Pulse;  		/* Toggle PortB at 500 Hz */ 
        Timer1_delay();  	/* Call delay which provide desired delay */    
    }   
}

void Timer1_delay()
{
    /* Enable 16-bit TMR1 register, No pre-scale, internal clock,timer OFF */
    //T1CON=0x80;  

    TMR1=0xf830;                /* Load count for generating delay of 1ms */
    T1CONbits.TMR1ON=1;         /* Turn ON Timer1 */
    while(PIR1bits.TMR1IF==0); 	/* Wait for Timer1 overflow interrupt flag */
    TMR1ON=0;                   /* Turn OFF timer */
    TMR1IF=0;                   /* Make Timer1 overflow flag to '0' */
}
