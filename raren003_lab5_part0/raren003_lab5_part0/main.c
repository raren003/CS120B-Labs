/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #4 Exercise #0
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/ 

#include <avr/io.h>


int main(void)
{
    DDRA = 0x00; PORTA = 0xFF; // Configure PORTA as input, initialize to 1s
    DDRB = 0xFF; PORTB = 0x00; // Configure PORTB as outputs, initialize to 0s
    unsigned char led = 0x00;
    unsigned char button = 0x00;
	
    while (1) 
    {
		// if PA0 is 1, set PB1PB0=01, else =10
		// 1) Read inputs
		button = ~PINA & 0x01;	//button is connected to A0
		// 2) Perform Computation
		if (button) //true if button is pressed
		{
			led = 0xFF; 
		}
		else {
			led = 0x00;	
		}
		// 3) Write outputs
		PORTC = led;
    }
}

