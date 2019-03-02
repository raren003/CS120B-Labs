/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu
* Lab Section: 022
* Assignment: Final Project
* Exercise Description: CS120B Final Project
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>
#include <avr/common.h>
#include <avr/interrupt.h>
#include "shiftreg_write.h"
#include "io.c"


typedef struct task {
	int state;
	unsigned long period;
	unsigned long elapsedTime;
	int (*TickFct)(int);
} task;

task tasks[1];
const unsigned short tasksNum = 1;
const unsigned long taskPeriodGCD = 300;
const unsigned long periodEnemyLED = 300;

	
unsigned char currentLED = 0x00;

enum ENEMYLED_STATES {EL_START, EL_INIT, EL_NEXTLED, EL_SELECTPRESS, EL_LEDLIT};
int TickFct_EnemyLED(int state){
	
	switch (state){			//transitions
		case EL_START:
			state = EL_INIT;
			break;
			
		case EL_INIT:
			state = EL_NEXTLED;
			break;
			
		case EL_NEXTLED:
			if (!(~PINA & 0x01)){
				state = EL_NEXTLED;
			} else if (~PINA & 0x01){
				state = EL_SELECTPRESS;
			}
			break;
			
		case EL_SELECTPRESS:
			if (~PINA & 0x01){
				state = EL_SELECTPRESS;
			}else if (!(~PINA & 0x01)){
				state = EL_LEDLIT;
			}
			break;
			
		case  EL_LEDLIT:
			if (!(~PINA & 0x01))
			{
				state = EL_LEDLIT;
			}else if (~PINA & 0x01){
				state = EL_NEXTLED;
			}
			break;
			 
		default:
			break;
	}					//Transitions
	
	switch (state){			//State actions
		case EL_START:
		state = EL_INIT;
		break;
		
		case EL_INIT:
			currentLED = 0x01;
			transmit_data(currentLED);
			break;
		
		case EL_NEXTLED:
			if (currentLED == 0x80) //go back to led 1 after 3 is lit
			{
				currentLED = 0x01;
			}
			else //shift to move to next led
			{
				currentLED = currentLED << 1;
			}
			transmit_data(currentLED);
			break;
		
		case EL_SELECTPRESS:
			transmit_data(currentLED);
			break;
		
		case  EL_LEDLIT:
			transmit_data(currentLED);
			break;
		
		default:
		break;
	}					//State Actions
	
	
	return state;
};

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
	DDRA = 0x00; PORTA = 0xFF;	// Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; // PORTB set to output, outputs init 0s
	
	unsigned char i = 0;
	tasks[i].state = EL_START;
	tasks[i].period = periodEnemyLED;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_EnemyLED;
	i++;
	
	TimerSet(taskPeriodGCD);
	TimerOn();
    
    while (1) 
    {
    }
}

