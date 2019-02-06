/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #7 Exercise #2
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>
#include "timer.h"
#include "io.c"

enum States {START, INIT, NEXTLED, LIT, WAIT} state;

unsigned char tmpB = 0x00; //variable for setting portB
unsigned char button = 0x00;
unsigned char score = 0x00;
unsigned char* winMessage = "YOU WINN!!!";


void Tick(){
	switch(state){			//transitions
		case START:
		score = 5;
		state = INIT;
		break;
		
		case INIT:
			if (!button) state = NEXTLED;
			break;
			
			case NEXTLED:
			state = NEXTLED;
			if (button) {
				
				if (tmpB == 0x02)
				{
					++score;
				}else{
					if (score > 0)
					{
						--score;
					}
				}
				
				state = LIT;
				
			}else if(!button){
				state = NEXTLED;
			}
		break;
		
		case LIT:
			if (!button) state = WAIT;
			else state = LIT;
		break;
		
		case WAIT:
			if (button && (score == 9)){ state = START; LCD_ClearScreen();}
			else if (button) state = INIT;
			else state = WAIT;
		break;
		
		default:
		break;
	}						//transitions
	
	switch(state){			//state actions
		case START:
		break;
		
		case INIT:
			tmpB = 0x01;
			PORTB = tmpB;
			LCD_Cursor(1);					//position cursor on the LCD display
			LCD_WriteData(score + '0');
		break;
		
		case NEXTLED:
			if (tmpB >= 0x04) //go back to led 1 after 3 is lit
			{
				tmpB = 0x01;
			}
			else //shift to move to next led
			{
				tmpB = tmpB * 2;
			}
			PORTB = tmpB;
		break;
		
		case LIT:
			LCD_Cursor(1);					//position cursor on the LCD display
			LCD_WriteData(score + '0');
			if (score == 9) {
				LCD_DisplayString(1, winMessage);
			
			}
		break;
		
		case WAIT:
			if (score == 9) {
				LCD_DisplayString(1, winMessage);
				
			}
		break;
		
		default:
		break;
	}						//state actions
}


int main(void)
{
	DDRB = 0xFF; PORTB = 0x00; //set PORTB as output initialize to 0
	DDRA = 0x00; PORTA = 0xFF;
	
	DDRC = 0xFF; PORTC = 0x00;	//LCD data lines
	DDRD = 0xFF; PORTD = 0x00;	//LCD control lines
	
	//initialize the LCD display
	LCD_init();
	
	state = START;
	TimerSet(300);
	TimerOn();
	button = ~PINA & 0x01;
	while (1)
	{

		Tick();
		button = ~PINA & 0x01;
		while (!TimerFlag){} //wait 1 sec
		TimerFlag = 0;
	}
}