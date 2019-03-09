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

#define SHIFTREGPORT PORTB

void transmit_data(unsigned char data){
	
	unsigned char i;
	
	
	//for each bit of data
	for (i = 0; i < 8; i++){
		//Set SRCLR to 1 allowing data to be set
		//Also clear SRCLK in preparation of sending  data
		SHIFTREGPORT = 0x08;
		//set SER = next bit of data to be sent
		SHIFTREGPORT |= ((data >> i) & 0x01);
		//set SRCLK = 1. Rising edge shifts next bit of data into the shifts register
		SHIFTREGPORT |= 0x04;
		
	}	//end for each bit of data
	
	//set RCLK = 1. Rising edge copies data from the "Shift" register to the "Storage" Register
	SHIFTREGPORT |= 0x02;
	//clear all lines in preparation of new data transmission
	SHIFTREGPORT = 0x00;
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


unsigned char lives;
unsigned char temp2;

enum Leader_STATES {leadr_start, leadr_ON, leadr_OFF};
int TickFct_Leader(int state){
	
	switch(state){			//Transitions
		
		case leadr_start:
			state = leadr_ON;
			break;
			
		case  leadr_ON:
			state = leadr_OFF;
			break;
			
		case leadr_OFF:
			state = leadr_ON;
			break;
			
		default:
			break;
		
	}						//Transitions
	
	switch(state){			//State actions
		
		case leadr_start:
			temp2 = 0x0F;
			lives = 4;
		break;
		
		case  leadr_ON:
				
			if(USART_IsSendReady()){
				USART_Send(temp2);
				transmit_data(temp2);
				USART_Flush();
			}
			break;
		
		case leadr_OFF:
		/*
			if(USART_IsSendReady()){
				USART_Send(0x00);
				transmit_data(0x00);
				USART_Flush();
			}
		*/
			if (lives > 0){
				lives--;
				temp2 = temp2 >> 1;
			}else{
				lives = 4;
				temp2 = 0x0F;
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
	tasks[i].state = leadr_start;
	tasks[i].period = taskPeriodFollow;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_Leader;
	
	TimerSet(taskPeriodGCD);
	TimerOn();
    
    while (1) 
    {
    }
}

