/*Name & E-mail: Robert Arenas, raren003@ucr.edu
* Partner(s) Name & E-mail: Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #2 Exercise #4
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>


int main(void)
{
    
	DDRA = 0x00; PORTA = 0xFF;	// Configure port A's 8 pins as inputs
	DDRB = 0x00; PORTB = 0xFF;	// Configure port B's 8 pins as inputs
	DDRC = 0x00; PORTC = 0xFF;	// Configure port C's 8 pins as inputs
	DDRD = 0xFF; PORTD = 0x00;	// Configure port D's 8 pins as outputs, Initialize 0’s
	unsigned char tmpA = 0x00;  // Temporary variable to hold the value of A
	unsigned char tmpB = 0x00;  // Temporary variable to hold the value of B
	unsigned char tmpC = 0x00;  // Temporary variable to hold the value of C
	
	unsigned char uc_dOut = 0x00;
	signed short uc_acWeightDiff = 0x00; //variable holds the weight difference between A and C 
	unsigned short us_weightActual;
	unsigned short us_weightShifted;
	
    while (1) 
    {
		// 1) Read input
		tmpA = PINA;
		tmpB = PINB;
		tmpC = PINC;
		
		uc_acWeightDiff = (short)tmpA - (short)tmpC;
		
		
		us_weightActual = (short)tmpA + (short)tmpB + (short)tmpC;
		
		if (us_weightActual > 140)
		{
			uc_dOut = 0x01;
		}else if(uc_acWeightDiff > 80 || uc_acWeightDiff < -80){
			uc_dOut = 0x02;
		}else{
			uc_dOut = 0x00;
		}
		
		PORTD = uc_dOut;
		
    }
	
}

