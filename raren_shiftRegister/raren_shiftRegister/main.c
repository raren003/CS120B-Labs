/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu
* Lab Section: 022
* Assignment: Shift Register
* Exercise Description: Learning to use a shift register for final project
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>

void transmit_data(unsigned char data){
	
	unsigned char i;
	
	
	//for each bit of data
	for (i = 0; i < 8; i++){
		//Set SRCLR to 1 allowing data to be set
		//Also clear SRCLK in preparation of sending  data
		PORTB = 0x08;
		//set SER = next bit of data to be sent
		PORTB |= ((data >> i) & 0x01);
		//set SRCLK = 1. Rising edge shifts next bit of data into the shifts register
		PORTB |= 0x04;
		
	}	//end for each bit of data
	
	//set RCLK = 1. Rising edge copies data from the "Shift" register to the "Storage" Register
	PORTB |= 0x02;
	//clear all lines in preparation of new data transmission
	PORTB = 0x00;
}



int main(void)
{
    DDRB = 0xFF; PORTB = 0x00; // PORTB set to output, outputs init 0s
	
	unsigned char x = 12;
	
	transmit_data(x);
	
    while (1) 
    {
		
    }
}

