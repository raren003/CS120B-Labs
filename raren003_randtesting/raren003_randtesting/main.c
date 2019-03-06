/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: 
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>
#include <stdlib.h>
#include "io.c"
#include "timer.h"

void transmit_data(unsigned char data){
	
	unsigned char i;
	
	
	//for each bit of data
	for (i = 0; i < 8; i++){
		//Set SRCLR to 1 allowing data to be set
		//Also clear SRCLK in preparation of sending  data
		PORTB = 0x08;
		//set SER = next bit of data to be sent
		PORTB |= ((data >> i) & 0x01);
		//set SRCLK = 1. Rising edge shifts next bit of data into the shifts register
		PORTB |= 0x04;
		
	}	//end for each bit of data
	
	//set RCLK = 1. Rising edge copies data from the "Shift" register to the "Storage" Register
	PORTB |= 0x02;
	//clear all lines in preparation of new data transmission
	PORTB = 0x00;
}

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00; // PORTB set to output, outputs init 0s
	DDRC = 0xFF; PORTC = 0x00;	//LCD data lines
	DDRD = 0xFF; PORTD = 0x00;	//LCD control lines
	//unsigned char buffer[20];
	const unsigned char pATTK = 6;
	unsigned char y;
	unsigned char* attk_String = "E-attk          P-attk";
	unsigned char* pattk_String = "P-attk";
	
	//initialize the LCD display
	LCD_init();
	
	TimerSet(1000);
	TimerOn();
	
	while (1)
	{ 
		/*
		itoa((rand()%(50-23) +23), buffer, 10);
		LCD_DisplayString(1, buffer); */
		
		//rand()%(maxValue-minValue)+minValue - range[minValue, maxValue]
		y = (unsigned char) (rand()%(10-0)+0);
		transmit_data(y);
		LCD_DisplayString(1, attk_String);
		LCD_Cursor(24); LCD_WriteData(6+'0');
		LCD_Cursor(8); LCD_WriteData(y+'0');
		while (!TimerFlag);
		TimerFlag = 0;
	}
}

