/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #5 Exercise #2
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>

enum States {Start, INIT, WAIT, INCREMENT, DECREMENT, HOLD} state;

unsigned char tmpB = 0x00; //counter variable for setting portB

void Tick(){
	
	
	switch(state) {				//transitions
		case Start:
		state = INIT;
		break;
		
		case INIT:
		state = WAIT;
		break;
		
		case WAIT:
		if(!(~PINA & 0x01) && !(~PINA & 0x02)){ //!A0 && !A1
			state = WAIT;
		}else if((~PINA & 0x01) && (~PINA & 0x02)){ //A0 && A1
			state = INIT;
		}else if((~PINA & 0x01) && !(~PINA & 0x02)){ //A0 && !A1
			state = INCREMENT;
		}else if(!(~PINA & 0x01) && (~PINA & 0x02)){ //!A0 && A1
			state = DECREMENT;
		}
		break;
		
		case INCREMENT:
		state = HOLD;
		break;
		
		case DECREMENT:
		state = HOLD;
		break;
		
		case HOLD:
		state = WAIT;
		if ((~PINA & 0x01) && (~PINA & 0x02)){ //A0 && A1
			state = INIT;
		}else if(!(~PINA & 0x01) && !(~PINA & 0x02)) { //!(A0 && A1)
			state = WAIT;
		}else if ((~PINA & 0x01) || (~PINA & 0x02)){ //A0 || A1
			state = HOLD;
		}
		break;
		
		default:
		break;
	}							//transitions
	
	switch(state){				//state actions
		case INIT:
		tmpB = 0;
		PORTB = tmpB;
		break;
		
		case WAIT:
		break;
		
		case INCREMENT:
		if (tmpB < 0x09)
		{
			tmpB = tmpB + 1;
		}
		PORTB = tmpB;
		break;
		
		case DECREMENT:
		if (tmpB > 0x00)
		{
			tmpB = tmpB - 1;
		}
		PORTB = tmpB;
		break;
		
		case HOLD:
		break;
		
		default:
		break;
	}								//state actions
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;	// Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00;	// Configure port B's 8 pins as outputs, Initialize 0’s
	
	state = Start;	//initial call
	
	while(1) {Tick();}
}