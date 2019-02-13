/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #10 Exercise #1
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/ 

#include <avr/io.h>
#include "timer.h"

static unsigned char threeLEDs = 0x00;
static unsigned char bilnkingLED =  0x00;

enum THREELED_STATES {Th_start,TH_Zer0On, TH_OneOn, TH_Two_on} THREELED_STATE;
void TickFct_ThreeLeds(){
	
	switch(THREELED_STATE){
		
		case Th_start:
			THREELED_STATE = TH_Zer0On;
		break;
		
		case  TH_Zer0On:
			THREELED_STATE = TH_OneOn;
		break;
		
		case TH_OneOn:
			THREELED_STATE = TH_Two_on;
		break;
		
		case  TH_Two_on:
			THREELED_STATE = TH_Zer0On;
		break;
		
		default:
		break;
	}
	
	switch(THREELED_STATE){ // state actions
		
		case Th_start:
		break;
		
		case  TH_Zer0On:
			threeLEDs = 0x01;
		break;
		
		case TH_OneOn:
			threeLEDs = 0x02;
		break;
		
		case  TH_Two_on:
			threeLEDs = 0x04;
		break;
		
		default:
		break;
	}			//state actions
	
}				
	
enum BLINKINGLED_STATES {BL_start,  BL_ThreeOn, BL_ZeroOn} BLINKINGLED_STATE;
void TickFct_BlinkingLeds(){
	
	switch(BLINKINGLED_STATE){
		case BL_start:
			BLINKINGLED_STATE = BL_ThreeOn;
		break;
		
		case BL_ThreeOn:
			BLINKINGLED_STATE = BL_ZeroOn;
		break;
		
		case  BL_ZeroOn:
			BLINKINGLED_STATE = BL_ThreeOn;
		break;
		
		default:
		break;
	}
	
	switch(BLINKINGLED_STATE){	//state actions
		case BL_start:
		break;
		
		case BL_ThreeOn:
			bilnkingLED = 0x08;
		break;
		
		case  BL_ZeroOn:
			bilnkingLED = 0x00;
		break;
		
		default:
		break;
	}
	
}
	
enum COMBINELED_STATES {CO_start, CO_combine } COMBINELED_STATE;
void TickFct_CombineLeds(){
	
	switch(COMBINELED_STATE){
		case CO_start:
			COMBINELED_STATE =  CO_combine;
		break;
		
		case CO_combine:
			COMBINELED_STATE = CO_combine;
		break;
	
	}
	
	switch(COMBINELED_STATE){
		case CO_start:
		break;
		
		case CO_combine:
			PORTB = threeLEDs | bilnkingLED;
		break;
		
	}
	
}


int main(void)
{
	DDRB = 0xFF; PORTB = 0x00; //set PORTB as output initialize to 0
	
	unsigned long ThreeLED_elapsedTime = 0;
	unsigned long BlinkingLed_elapsedTime = 0;
	unsigned long CombineLED_elapsedTime = 0;
	const unsigned long timerPeriod = 1000;
	
	
	TimerSet(1000);
	TimerOn();
	
	THREELED_STATE = Th_start;
	BLINKINGLED_STATE = BL_start;
	COMBINELED_STATE = CO_start;
   
    while (1) 
    {
		if (ThreeLED_elapsedTime >= 1000)
		{
			TickFct_ThreeLeds();
			ThreeLED_elapsedTime = 0;
		}
		
		if (BlinkingLed_elapsedTime >= 1000)
		{
			TickFct_BlinkingLeds();
			BlinkingLed_elapsedTime = 0;
		}
		
		if(CombineLED_elapsedTime >= 1000){
			TickFct_CombineLeds();
			CombineLED_elapsedTime = 0;
		}
		while (!TimerFlag){}
		TimerFlag = 0;
		ThreeLED_elapsedTime += timerPeriod;
		BlinkingLed_elapsedTime += timerPeriod;
		CombineLED_elapsedTime += timerPeriod;
    }
}

