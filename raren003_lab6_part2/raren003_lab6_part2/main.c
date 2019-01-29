/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #6 Exercise #2
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>
#include "timer.h"

enum States {START, INIT, NEXTLED, LIT,	WAIT} state;

unsigned char tmpB = 0x00; //variable for setting portB

void Tick(){
	switch(state){			//transitions
		case START:
		state = INIT;
		break;
		
		case INIT:
		if(~PINA & 0x01){
			state = INIT;
		}else if (!(~PINA & 0x01))
		{
			state = NEXTLED;
		}
		break;
		
		case NEXTLED:
		if (!(~PINA & 0x01))
		{
			state = NEXTLED;
		}else if (~PINA & 0x01)
		{
			state = LIT;
		}
		break;
		
		case LIT:
		if (~PINA & 0x01)
		{
			state = LIT;
		} 
		else if(!(~PINA & 0x01))
		{
			state = WAIT;
		}
		break;
		
		case WAIT:
		if(!(~PINA & 0x01)){
			state = WAIT;
		}else if (~PINA & 0x01)
		{
			state = INIT;
		}
		
		default:
		break;
	}						//transitions
	
	switch(state){			//state actions
		case START:
		break;
		
		case INIT:
		tmpB = 0x01;
		PORTB = tmpB;
		break;
		
		case NEXTLED:
		if (tmpB == 0x04) //go back to led 1 after 3 is lit
		{
			tmpB = 0x01;
		}
		else //shift to move to next led
		{
			tmpB = tmpB << 1;
		}
		PORTB = tmpB;
		break;
		
		case LIT:
		PORTB = tmpB;
		break;
		
		case WAIT:
		PORTB = tmpB;
		break;
		
		default:
		break;
	}						//state actions
}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;	// Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; //set PORTB as output initialize to 0
	
	TimerSet(300);
	TimerOn();
	
	state = START;
	
	while (1)
	{
		Tick();
		while (!TimerFlag){} //wait 1 sec
		TimerFlag = 0;
	}
}