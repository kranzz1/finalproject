/*
 * File:   tempsensor.c
 * Author: zkran
 *
 * Created on April 21, 2023, 2:10 PM
 */

/*Preprocessor Directives*/
#include <xc.h>
#include <pic18f4520.h>             // for TRISB and PORTB declarations



/*Preprocessor Macros*/

#define _XTAL_FREQ 1000000     //Set XC8 oscillator freq


/*Configuration Bits*/

#pragma config OSC = INTIO7     //Oscillator Selection bits (internal)
#pragma config WDT = OFF        // Watchdog timer off
#pragma config LVP = OFF        // Low-voltage programming off
#pragma config PBADEN = OFF     // PORTB<4:0> pins configured as digital I/O


/*Function Prototypes*/


/*Global Variables*/

void main(void) {
    while(1)
    {
        
    }
}