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
#include "timer.h"


int main(void)
{
    DDRC = 0xFF; PORTC = 0x00;	//LCD data lines
	DDRD = 0xFF; PORTD = 0x00;	//LCD control lines
	
	unsigned char* LCD_Output = " CS120B is Legend... wait for it DARY!";
	unsigned char i = 0;
	
	//initialize the LCD display
	LCD_init();
	
	TimerSet(350);
	TimerOn();
	
	//starting at position 1 on the LCD screen, writes Hello World
	//LCD_DisplayString(1, LCD_Output);
	
	LCD_Cursor(1);
	
    while (1) 
    {
		//LCD_DisplayString(1, LCD_Output);
		
		for (i=32; i>0; i--)
		{
			LCD_DisplayString(i, LCD_Output);
			while (!TimerFlag);
			TimerFlag = 0;
			
		}
		i = 32;
	 }
}

