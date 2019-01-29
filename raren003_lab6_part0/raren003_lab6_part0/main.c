/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #6 Exercise #0
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>
#include "timer.h"


int main(void)
{
    DDRB = 0xFF; //set port B to outputs
	PORTB = 0x00; //inti port B to 0's
	TimerSet(1000);
	TimerOn();
	unsigned char tmpB = 0x00;
	
    while (1) 
    {
		//User code (i.e. synchSM calls)
		tmpB = ~tmpB; //Toggle PORTB; Temporary, bad programming style
		PORTB = tmpB;
		while(!TimerFlag); //Wait 1 sec
		TimerFlag = 0;
		//Note: For the above a better style would use a SynchSM with TickSM()
		//This example just illustrated the use of the ISR and flag
    }
}

