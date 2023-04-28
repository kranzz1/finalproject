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
void xmitInProgress(void);

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
    SSPADD = 0x19;              //SSPADD value for I2C clk
    SSPSTATbits.SMP = 1;        //Disable slew rate control
    SSPCON1bits.SSPEN = 1;      //Master Sync. Serial Port enable
    SSPCON1bits.SSPM = 1000;       
    SSPCON2 = 0x00;
    
    
    //Set pin directions
    TRISCbits.RC3 = 1;          //SCL
    TRISCbits.RC4 = 1;          //SDA
    
    //Interrupt Initialization
    
    /*
    INTCONbits.RBIF = 0;        //Reset PortB Interrupt flag
    INTCONbits.RBIE = 1;        //enable port b change interrupts
    RCONbits.IPEN = 1;          //enable interrupt priority
    INTCON2bits.RBIP = 1;       //High priority
    INTCONbits.GIEH = 1;        //enable all high interrupts
    INTCONbits.PEIE = 1;        //peripheral interrupts
     */
    
    RCONbits.IPEN = 1;
    INTCON2bits.INTEDG0 = 1;
    INTCONbits.INT0IE = 1;
    INTCONbits.INT0IF = 1;
    INTCONbits.GIEH = 1;
   

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
            
            LCD_cmd(0xCF);
            LCD_data(update + '0');
            update = 0;
        }
        LCD_cmd(0xCF);
            LCD_data(update + '0');
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
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
    SSPCON2bits.SEN=1;      //start condition enable
    xmitInProgress();
    SSPBUF=I2C_SLAVE_ADDR;            //send tempsensor I2C address
    xmitInProgress();
    SSPBUF=0x00;            //send tempsensor readtemp command
    xmitInProgress();
    SSPCON2bits.RSEN = 1;   //generate restart condition
    while ( SSPCON2bits.RSEN );     // wait until re-start condition is over 
    SSPBUF=I2C_SLAVE_ADDR;            //send tempsensor I2C address
    xmitInProgress();
    SSPCON2bits.ACKDT=1;    //To read the data byte, the ACKDT bit is first set to indicate that a NO ACK should be sent.
    SSPCON2bits.RCEN=1;     //Then the RCEN bit is set to initialize the read and the data can be copied from SSPBUF
    xmitInProgress();
    rawtemp = SSPBUF;
    while ( SSPCON2bits.ACKEN );
    SSPCON2bits.PEN=1;
    while (SSPSTATbits.P);
    return rawtemp;
}
void xmitInProgress(void){
    
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F)); //Transmit is in progress
    
    while(!PIR1bits.SSPIF);
    
    PIR1bits.SSPIF=0;
    
}
void __interrupt(high_priority) high_ISR(void)
{
    if(INTCONbits.INT0IF == 1)
    {
        tempdata = queryTemp();
        update = 1;
        __delay_ms(100);
        INTCONbits.INT0IF = 0;        //Reset PortB Interrupt flag
    }
}