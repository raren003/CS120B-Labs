/*
 * raren003_lab2_part2.c
 *
 * Created: 1/9/2019 9:09:28 PM
 * Author : Rob
 */ 

#include <avr/io.h>


int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;	// Configure port A's 8 pins as inputs
    DDRC = 0xFF; PORTC = 0x00;	// Configure port C's 8 pins as outputs, Initialize 0’s
    unsigned char tmpA = 0x00;  // Temporary variable to hold the value of A
	unsigned char cntavail = 0x00;
    while (1)
    {
	    // 1) Read input
	    tmpA = PINA;
	    if(tmpA == 0x0F){
		    cntavail = (cntavail & 0x00) | 0x00;
	    }
		else if(tmpA == 0x07 || tmpA == 0x0B || tmpA == 0x0D || tmpA == 0x0E){
			cntavail = (cntavail & 0x00) | 0x01;
		}
		else if(tmpA == 0x03 || tmpA == 0x05 || tmpA == 0x06 || tmpA == 0x09 || tmpA == 0x0A || tmpA == 0x0C){
			cntavail = (cntavail & 0x00) | 0x02;
		}
		else if(tmpA == 0x01 || tmpA == 0x02 || tmpA == 0x04 || tmpA == 0x08){
			cntavail = (cntavail & 0x00) | 0x03;
		}
		else if(tmpA == 0x00){
			cntavail = (cntavail & 0x00) | 0x04;
		}
	    else{
		    cntavail = (cntavail & 0x00) | 0xFF;
	    }
	    
	    //write output
	    PORTC = cntavail;
    }
}

