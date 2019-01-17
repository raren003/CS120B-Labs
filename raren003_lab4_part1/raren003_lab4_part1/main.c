/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #4 Exercise #1
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/


#include <avr/io.h>

enum States {Start, FIRST_ON, SECOND_ON} state;

void Tick() {
	switch(state) {				//transitions
		case Start:
			state = FIRST_ON;
			break;
			
		case FIRST_ON:
			if(!(PINA & 0x01)){
				state = FIRST_ON;
			}else if((PINA & 0x01)){
				state = SECOND_ON;
			}
			break;
			
		case SECOND_ON:
			if((PINA & 0x01)){
				state = SECOND_ON;
			}else if(!(PINA & 0x01)){
				state = FIRST_ON;
			}
			break;
		
		default:
			break;
	}							//transitions
	
	switch(state) {				//state actions
		case FIRST_ON:
			PORTB = 0x01;
			break;
		
		case SECOND_ON:
			PORTB = 0x02;
			break;
		
		default:
			break;
	}							//state actions
	
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;	// Configure port A's 8 pins as inputs
	DDRB = 0x00; PORTB = 0x00;	// Configure port B's 8 pins as outputs, Initialize 0’s
	
	state = Start;	//initial call
	
	while(1) {Tick();}
}
