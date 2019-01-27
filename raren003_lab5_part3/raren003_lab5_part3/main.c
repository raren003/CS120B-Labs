/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #5 Exercise #3
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>

enum States {Start, BEGIN_DISPLAY, CONFIG_ONE, CONFIG_TWO, CONFIG_THREE} state;
	
void Tick(){
	
	
	switch(state) {				//transitions
		case Start:
		state = BEGIN_DISPLAY;
		break;
		
		case BEGIN_DISPLAY:
		if (!(~PINA & 0x01)){	//!A0
			state = BEGIN_DISPLAY;
		}
		else if ((~PINA & 0x01)){	//A00
			state = CONFIG_ONE;
		}
		break;
		
		case CONFIG_ONE:
		if (!(~PINA & 0x01)){	//!A0
			state = CONFIG_ONE;
		} 
		else if ((~PINA & 0x01)){	//A00
			state = CONFIG_TWO;
		}
		break;
		
		case CONFIG_TWO:
		if (!(~PINA & 0x01)){	//!A0
			state = CONFIG_TWO;
		}
		else if ((~PINA & 0x01)){	//A00
			state = CONFIG_THREE;
		}
		break;
		
		case CONFIG_THREE:
		if (!(~PINA & 0x01)){	//!A0
			state = CONFIG_THREE;
		}
		else if ((~PINA & 0x01)){	//A00
			state = BEGIN_DISPLAY;
		}
		break;
		
		default:
		break;
	}							//transitions
	
	switch(state){				//state actions
		
		case Start:
		break;
		
		case BEGIN_DISPLAY:
		PORTC = 0x00;
		break;
		
		case CONFIG_ONE:
		PORTC = 0x21;
		break;
		
		case CONFIG_TWO:
		PORTC = 0x12;
		break;
		
		case CONFIG_THREE:
		PORTC = 0x0C;
		break;
		
		
		default:
		break;
	}								//state actions
}

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;	// Configure port A's 8 pins as inputs
    DDRC = 0xFF; PORTC = 0x00;	// Configure port B's 8 pins as outputs, Initialize 0’s
	
	state = Start;	//initial call
	
    while (1) 
    {
		Tick();
    }
}

