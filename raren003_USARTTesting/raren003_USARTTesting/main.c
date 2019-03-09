/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu
* Lab Section: 022
* Assignment: experimenting with usart
* Exercise Description:
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>
#include <avr/common.h>
#include <avr/interrupt.h>
#include "usart.h"

//PORT connected to 7 segment diode
//A-pin0, B-pin1, C-pin2, D-pin3, E-pin4, F-pin5, G-pin6, DP-pin7
#define SSEGPORT PORTB

void sseg_write(unsigned char data){
	switch(data){
		case 0:
			SSEGPORT = 0xC0;
			break;
		case 1:
			SSEGPORT = 0xF9;
			break;
		case 2:
			SSEGPORT = 0xA4;
			break;
		case 3:
			SSEGPORT = 0xb0;
			break;
		case 4:
			SSEGPORT = 0x99;
			break;
		case 5:
			SSEGPORT = 0x92;
			break;
		case 6:
			SSEGPORT = 0x82;
			break;
		case 7:
			SSEGPORT = 0xF8;
			break;
		case 8:
			SSEGPORT = 0x80;
			break;
		case 9:
			SSEGPORT = 0x98;
			break;
		default:
			SSEGPORT = 0x83;
			break;
	}
	
}


typedef struct task {
	int state;
	unsigned long period;
	unsigned long elapsedTime;
	int (*TickFct)(int);
} task;

task tasks[1];
const unsigned short tasksNum = 1;
const unsigned long taskPeriodGCD = 1000;
const unsigned long taskPeriodFollow = 1000;


unsigned char temp;

enum Follower_STATES {FOLW_start, FOLW_ON};
int TickFct_FOLLOWER(int state){
	
	switch(state){			//Transitions
		
		case FOLW_start:
			state = FOLW_ON;
			break;
			
		case  FOLW_ON:
			state = FOLW_ON;
			break;
			
		default:
			break;
		
	}						//Transitions
	
	switch(state){			//State actions
		
		case FOLW_start:
		break;
		
		case  FOLW_ON:
			if (USART_HasReceived()){
				temp = USART_Receive();
				sseg_write(temp);
				USART_Flush();
			}
		break;
		
		default:
		break;
		
	}						//State Actions
	
	return state;
}

///////////////////////////////////////////////////////////////
////////////FUNCTIONS FOR THE TIMER////////////////////////////
///////////////////////////////////////////////////////////////

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1ms
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B 	= 0x0B;	// bit3 = 1: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: prescaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A 	= 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register

	TIMSK1 	= 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1 = 0;

	// TimerISR will be called every _avr_timer_cntcurr milliseconds
	_avr_timer_cntcurr = _avr_timer_M;

	//Enable global interrupts
	SREG |= 0x80;	// 0x80: 1000000
}

void TimerOff() {
	TCCR1B 	= 0x00; // bit3bit2bit1bit0=0000: timer off
}

/*void TimerISR() {
	TimerFlag = 1;
}*/

void TimerISR(){
	unsigned char i;
	for (i = 0; i <tasksNum; ++i){
		if ( tasks[i].elapsedTime >= tasks[i].period){
			tasks[i].state = tasks[i].TickFct(tasks[i].state);
			tasks[i].elapsedTime = 0;
		}
		tasks[i].elapsedTime += taskPeriodGCD;
	}
	//TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect)
{
	// CPU automatically calls when TCNT0 == OCR0 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; 			// Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { 	// results in a more efficient compare
		TimerISR(); 				// Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}


int main(void)
{
	DDRB = 0xFF; PORTB = 0x00; //set B as output
	
	initUSART();
	
	unsigned char i = 0;
	tasks[i].state = FOLW_start;
	tasks[i].period = taskPeriodFollow;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_FOLLOWER;
	
	TimerSet(taskPeriodGCD);
	TimerOn();
    
    while (1) 
    {
    }
}

