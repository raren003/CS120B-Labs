/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #7 Exercise #0
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>
#include "io.c" 


int main(void)
{
    DDRC = 0xFF; PORTC = 0x00;	//LCD data lines
	DDRD = 0xFF; PORTD = 0x00;	//LCD control lines
	
	unsigned char* LCD_Output = "Hello World";
	
	//initialize the LCD display
	LCD_init();
	
	//starting at position 1 on the LCD screen, writes Hello World
	LCD_DisplayString(1, LCD_Output);
	
    while (1) 
    {
		continue;
    }
}

