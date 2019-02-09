/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #9 Exercise #1
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>

// 0.954 hz is the lowest frequency possible with this function,
// based on settings in PWM_on
// Passing in 0 as the frequency will stop the speaker from generating sound
void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency){
		if (!frequency){ TCCR0B &= 0x08; } // stops timer/counter
		else { TCCR0B |= 0x03; } // resumes/continues timer/counter
			
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR0A = 0xFFFF; }
			
		// prevents OCR0A from underflowing, using prescaler 64
		// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250){ OCR0A = 0x0000;}
			
		// set OCR3A based on desired frequency
		else { OCR0A = (short)(8000000 / (128 * frequency)) - 1; }
			
		TCNT0 = 0; //reset counter
		current_frequency = frequency; // Updates the current frequency	
	}
		
}

void PWM_on() {
	TCCR0A = (1 << COM0A0) | (1 << WGM00);
			// COM3A0: Toggle PB3 on compare match between counter and OCR0A
	TCCR0B = (1 << WGM02) | (1 << CS01) | (1 << CS00);
			// WGM02: When counter (TCNT0) matches OCR0A, reset counter
			// CS01 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR0A = 0x00;
	TCCR0B = 0x00;
}

enum States {START, WAIT, A0Sound, A1Sound, A2Sound} state;
unsigned char button = 0x00; //used to check which button is being pressed based on the value of
							// of PORTA.

void Tick(){
	switch(state){			//Transitions
		case START:
			state = WAIT;
		break;
		
		case WAIT:
			if (button != 1 && button!= 2 && button != 4){ //same as !A0 && !A1 && !A2
				state = WAIT;
			}else if (button == 1){ //same as A0 && !A1 && !A2
				state = A0Sound;
			}else if (button == 2){ //same as !A0 && A1 && !A2
				state = A1Sound;
			}else if (button == 4){ //same as !A0 && !A1 && A2
				state = A2Sound;
			}
		break;
		
		case A0Sound:
			if(button == 1){	//same as A0 && !A1 && !A2
				state = A0Sound;
				state = A0Sound;
			}else if (button != 1){	//same as !A0
				state = WAIT;
			}
		break;
		
		case A1Sound:
			if(button == 2){	//same as !A0 && A1 && !A2
				state = A1Sound;
			}else if (button != 2){	//same as !A1
				state = WAIT;
			}
		break;
		
		case A2Sound:
			if(button == 4){	//same as !A0 && !A1 && A2
				state = A2Sound;
			}else if (button != 4){	//same as !A2
				state = WAIT;
			}
		break;
		
		default:
		break;
	}						//Transitions
	
	
	switch(state){			//state actions
		case START:
		break;
		
		case WAIT:
			set_PWM(0);
		break;
		
		case A0Sound:
			set_PWM(261.63);
		break;
		
		case A1Sound:
			set_PWM(293.66);
		break;
		
		case A2Sound:
			set_PWM(329.63);
		break;
		
		default:
		break;
	}						//State actions
}


int main(void)
{
	DDRB = 0x08; PORTB = 0x00; //set PB3 on PORTB as output
	DDRA = 0x00; PORTA = 0xFF; //set PORTA as input
	
	PWM_on();
	
	state = START;
	
    /* Replace with your application code */
    while (1) 
    {
		Tick();
		button = ~PINA;
		
    }
}

