/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #8 Exercise #4
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>


void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//		  in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}


int main(void)
{
	DDRB = 0xFF; PORTB = 0x00; //set PORTB as output initialize to 0
	DDRD = 0xFF; PORTD = 0x00; //set PORTD as output initialize to 0
	
	unsigned short us_ADC; //variable to read 10-bit ADC result
	
	ADC_init(); //initialize A/D converter
	
	
	/* Replace with your application code */
	while (1)
	{
		us_ADC = ADC;
		
		if (us_ADC >= 0 && us_ADC <= 240)
		{
			PORTB = 0x01;	//1 led
		}else if (us_ADC >= 240 && us_ADC <= 360)
		{
			PORTB = 0x03;	//2 led's
		}else if (us_ADC >= 360 && us_ADC <= 480)
		{
			PORTB = 0x07;	//3 led's
		}else if (us_ADC >= 480 && us_ADC <= 600)
		{
			PORTB = 0x0F;	//4 led's
		}else if (us_ADC >= 600 && us_ADC <= 720)
		{
			PORTB = 0x1F;	//5 led's
		}else if (us_ADC >= 720 && us_ADC <= 840)
		{
			PORTB = 0x3F;	//6 led's
		}else if (us_ADC >= 840 && us_ADC <= 960)
		{
			PORTB = 0x7F;	//7 led's
		}else if (us_ADC >= 960)
		{
			PORTB = 0xFF;	//8 led's
		}
		
	}
}

