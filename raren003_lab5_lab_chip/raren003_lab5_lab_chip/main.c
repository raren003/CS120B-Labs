/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #5 Exercise pre-lab lab_chip
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/ 

#include <avr/io.h>


int main(void)
{
    DDRB = 0xFF; PORTB = 0x00; //configure port B's 8 pins as outputs
    while (1) 
    {
		PORTB = 0x0F;	// Writes port B's 8 pins with 00001111
    }
}

