#include <xc.h>
#include <string.h>
#include <pic18f4520.h>             // for TRISB and PORTB declarations
#include "LCD_lib.h"


#define  _XTAL_FREQ 1000000 //setting the XC8 macro for oscillator frequency


void LCD_cmd(char cmd)
{
    LATD = cmd;
    LATCbits.LATC0 = 0;         //Set RS bit to low
    LATCbits.LATC1 = 1;         //Set enable bit high
    __delay_ms(10);
    LATCbits.LATC1 = 0;         //set enable bit low
    __delay_ms(10);
    return;
}



void LCD_data(char data)
{
    LATD = data;
    LATCbits.LATC0 = 1;         //Set RS bit to high
    LATCbits.LATC1 = 1;         //Set enable bit high
    __delay_ms(1);
    LATCbits.LATC1 = 0;         //set enable bit low
    __delay_ms(1);
    return;
}

void LCD_txt(unsigned char txt[])
{
    int z = strlen(txt);
    for(int i=0;i<z; i++)
    {
        LCD_data(txt[i]);
    }
    return;
}

void LCD_clear()
{
    __delay_ms(1);
    LCD_cmd(0x01);
    __delay_ms(1);
    
}

void LCD_init(void)
{
    __delay_ms(100);
    TRISC = 0x00;
    TRISD = 0x00;
    LCD_cmd(0x30);
    __delay_ms(10);
    LCD_cmd(0x30);
    __delay_ms(10);
    LCD_cmd(0x30);
    __delay_ms(10);
    LCD_cmd(0x3C);          //Set display to 8-bit mode, 2 lines, 5x10 font
    __delay_ms(5);
    LCD_cmd(0x08);          //Turn cursor, display, and blinking off
    __delay_ms(5);
    LCD_cmd(0x01);          //clear display
    __delay_ms(5);
    LCD_cmd(0x06);          // Increment cursor w/ no display shift
    __delay_ms(5);
    
    LCD_cmd(0x0C);          //turn display on and cursor blinking off
    
    return;
}

void LCD_screenWrite(unsigned char upper[], unsigned char lower[])
{
    LCD_clear();
    LCD_cmd(0x80);
    LCD_txt(upper);
    LCD_cmd(0xC0);
    LCD_txt(lower);
}

