/*
 * File:   main.c
 * Author: Willem
 * copied from: https://pastebin.com/f70wPPKT
 * Created on April 22, 2013, 2:23 PM
 */
#include <xc.h>
#include <pic12f675.h>
#include <stdio.h>
#include <stdlib.h>
/*
 *
 */
 
// CONFIG
#pragma config FOSC = INTRCIO  // Oscillator Selection bits (INTOSC oscillator: CLKOUT function on GP4/OSC2/CLKOUT pin, I/O function on GP5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-Up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // GP3/MCLR pin function select (GP3/MCLR pin function is digital I/O, MCLR internally tied to VDD)
#pragma config BOREN = OFF      // Brown-out Detect Enable bit (BOD disabled)
#pragma config CP = OFF         // Code Protection bit (Program Memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
 
// ========================================================================
// ===== DEFINITIONS ======================================================
// ========================================================================
 
// ===== Constants ========================================================
#define     Tx_Pin      GP4     // Don't change this
#define     BAUD        9600    // Adjust baud here
 
#define     _XTAL_FREQ  4.0
#define     uart_time_delay()   __delay_us(1000000/BAUD)  // Delay = 1/Baud
 
// ===== Prototypes ======================================================
 
// Initializations
void setup_variables(void);
void setup_ports(void);
 
// Subroutines
void read_sensor(void);
void uart_tx_bit_bang(unsigned char val);
 
// ===== Variables =======================================================
unsigned char val;
unsigned int sysclock;
 
// ========================================================================
// ===== Main =============================================================
// ========================================================================
int main(void) {
    // Initializations
    setup_variables();
    setup_ports();
   
    // Main Loop
    while(1) {
        read_sensor();
        uart_tx_bit_bang(val);
    }
}
 
// ========================================================================
// ===== Subroutines ======================================================
// ========================================================================
// Read analog sensor and do A/D conversion
void read_sensor(void) {
    val = 22;
}
 
// Method for transmitting value using serial bit banging
void uart_tx_bit_bang(unsigned char val) {
    unsigned char i;
    Tx_Pin = 0;                         // Start bit
    uart_time_delay();
    for ( i = 8 ; i != 0 ; --i ) {
        if (val & 0x01) Tx_Pin = 1;   // Begin with MSB
        else            Tx_Pin = 0;
        val >>= 1;
        uart_time_delay();
        }
    Tx_Pin = 1;                         // Stop bit
    uart_time_delay();
}
 
// ========================================================================
// ===== Setup Routines ===================================================
// ========================================================================
// Set variables to initial states
void setup_variables(void) {
    Tx_Pin = 1;
    val = 0;
}
 
void setup_ports(void) {
    TRISIO = 0x11001101;
    OPTION_REG = 0x11010010;
    ANSEL = 0x01010001;
    TMR0 = 0;
    GPIO = 0;
    CMCON = 0x00000111;
    ADON = 0x1;
    ADFM = 0x1;
}
