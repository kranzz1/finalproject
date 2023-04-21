#ifndef LCD_LIB_H
#define LCD_LIB_H
#include "pic18f4520.h"   // for TRISB and PORTB declarations 

/* Preprocessor Macros */
#define RS      LATCbits.LATC0 // setup RS for LCD as PORTC.bit0
#define E       LATCbits.LATC1 // setup E for LCD as PORTC.bit1
#define LCD_DB  LATD    //setup DB for LCD as PORTD

/* Function Prototypes */
void LCD_init(void);
void LCD_cmd(char); 
void LCD_data(char);
void LCD_txt(unsigned char []); 
void LCD_clear(void); 
void LCD_screenWrite(unsigned char [], unsigned char[]);

#endif //LCD_LIB_H