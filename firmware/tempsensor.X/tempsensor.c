/*
 * File:   tempsensor.c
 * Author: zkran
 *
 * Created on April 21, 2023, 2:10 PM
 */

/*Preprocessor Directives*/
#include <xc.h>
#include <pic18f4520.h>             // for TRISB and PORTB declarations
#include "LCD_lib.h"



/*Preprocessor Macros*/

#define _XTAL_FREQ 1000000     //Set XC8 oscillator freq
#define I2C_SLAVE_ADDR 0x4D

/*Configuration Bits*/

#pragma config OSC = INTIO7     //Oscillator Selection bits (internal)
#pragma config WDT = OFF        // Watchdog timer off
#pragma config LVP = OFF        // Low-voltage programming off
#pragma config PBADEN = OFF     // PORTB<4:0> pins configured as digital I/O


/*Function Prototypes*/
void updateLCD(int);
int queryTemp();
void writeStaticLCD();
void clear_sspif();

/*Global Variables*/
int tempdata;
int update;


void main(void) {
    //Setup I/O
    TRISA = 0x00;
    TRISB = 0x00;
    TRISC = 0x00;
    TRISD = 0x00;
    TRISBbits.RB0 = 1;
    
     //I2C Initialization
    SSPADD = 0x05;              //SSPADD value for I2C clk
    SSPCON1bits.SSPM = 1000;    //
    
    //Interrupt Initialization
    INTCONbits.RBIF = 0;        //Reset PortB Interrupt flag
    RCONbits.IPEN = 1;          //enable interrupt priority
    INTCON2bits.RBIP = 1;       //High priority
    INTCONbits.GIEH = 1;        //enable all high interrupts
    
   

    //Initialization for LCD
    LCD_init();
    LCD_clear();
    writeStaticLCD();
    
    tempdata = 123;
    update = 1;
    
    while(1)
    {
        if(update == 1)
        {
            updateLCD(tempdata);
            update = 0;
        }
    }
     
}
void updateLCD(int temp)
{
    char td3 =  (temp % 10) + '0';
    char td2 =  (temp % 100)/10 +'0';
    char td1 =  (temp % 1000)/100 +'0';
    LCD_cmd(0xC0);
    LCD_data(td1);
    LCD_data(td2);
    LCD_data(td3);
}
void writeStaticLCD()
{
    LCD_cmd(0x80);
    LCD_txt("Temp:");
    LCD_cmd(0xC3);
    LCD_txt("F");
}
int queryTemp()
{
    int rawtemp;
    SSPCON2bits.SEN = 1;        //start condition enable
    clear_sspif();
    SSPBUF = I2C_SLAVE_ADDR ;
    clear_sspif();
    while(SSPCON2bits.ACKSTAT = 0){}
    SSPBUF = 0x00;                  //read temp command
    clear_sspif();
    while(SSPCON2bits.ACKSTAT = 0){}
    SSPCON2bits.SEN = 1;        //start condition enable
    clear_sspif();
    SSPBUF = I2C_SLAVE_ADDR ;
    clear_sspif();
    while(SSPCON2bits.ACKSTAT = 0){}
    SSPCON2bits.RCEN = 1;           //RCEN
    clear_sspif();
    rawtemp = SSPBUF;
    SSPCON2bits.ACKDT = 1;      //NACK
    SSPCON2bits.PEN = 1;            //stop condition enable
    clear_sspif();
    return rawtemp;
}
void clear_sspif()
{
    while(PIR1bits.SSPIF = 0){}
    PIR1bits.SSPIF = 0;
}

void __interrupt(high_priority) high_ISR(void)
{
    if(INTCONbits.RBIF == 1)
    {
        __delay_ms(10);
        if(INTCONbits.RBIF == 1)
        {
        tempdata = 456;
        update  = 1;
        }  
    }
}