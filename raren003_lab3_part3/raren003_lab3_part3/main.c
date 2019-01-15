/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #3 Exercise #3
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>

// Bit-access function
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}



int main(void)
{
	
	DDRA = 0x00; PORTA = 0xFF;	// Configure port A's 8 pins as inputs
	DDRB = 0x00; PORTB = 0xFF;	// Configure port B's 8 pins as inputs
	DDRC = 0x00; PORTC = 0xFF;	// Configure port C's 8 pins as inputs
	DDRD = 0x00; PORTD = 0x00;	// Configure port D's 8 pins as outputs, Initialize 0�s
	unsigned char tmpA = 0x00;  // Temporary variable to hold the value of A
	
	unsigned char uc_cOut;
	while (1)
	{
		uc_cOut = 0;
		tmpA = PINA;
		
		if ((tmpA  & 0x0F) >= 1) uc_cOut = SetBit(uc_cOut, 5, 1);
		if ((tmpA  & 0x0F) >= 3) uc_cOut = SetBit(uc_cOut, 4, 1);
		if ((tmpA  & 0x0F) >= 5) uc_cOut = SetBit(uc_cOut, 3, 1);
		if ((tmpA  & 0x0F) >= 7) uc_cOut = SetBit(uc_cOut, 2, 1);
		if ((tmpA  & 0x0F) >= 10) uc_cOut = SetBit(uc_cOut, 1, 1);
		if ((tmpA  & 0x0F) >= 13) uc_cOut = SetBit(uc_cOut, 0, 1);
		if ((tmpA  & 0x0F) <= 4) uc_cOut = SetBit(uc_cOut, 6, 1);
		
		//sets PC7 to 1, if a key is in the ignition (P4), the driver is seated (P5), 
		//but the belt is not fastened (P6), aka (P4 && P5 && !P6)
		if(GetBit(tmpA, 4) &&  GetBit(tmpA, 5) && !GetBit(tmpA, 6)){
			 uc_cOut = SetBit(uc_cOut, 7, 1);
		}else{
			 uc_cOut = SetBit(uc_cOut, 7, 0);
		}
		
		PORTC = uc_cOut;
	}
}

