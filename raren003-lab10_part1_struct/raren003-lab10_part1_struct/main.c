/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #10 Exercise #1 W/struct task
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>
#include <avr/common.h>
#include <avr/interrupt.h>

typedef struct task {
	int state;
	unsigned long period;
	unsigned long elapsedTime;
	int (*TickFct)(int);
} task;

task tasks[3];
const unsigned short tasksNum = 3;
const unsigned long taskPeriodGCD = 1000;
const unsigned long periodThreeLED = 1000;
const unsigned long periodBlinkingLED = 1000;
const unsigned long periodCombineLED = 1000;


static unsigned char threeLEDs = 0x00;
static unsigned char bilnkingLED =  0x00;

enum THREELED_STATES {Th_start,TH_Zer0On, TH_OneOn, TH_Two_on}E;
int TickFct_ThreeLeds(int state){
	
	switch(state){
		
		case Th_start:
		state = TH_Zer0On;
		break;
		
		case  TH_Zer0On:
		state = TH_OneOn;
		break;
		
		case TH_OneOn:
		state = TH_Two_on;
		break;
		
		case  TH_Two_on:
		state = TH_Zer0On;
		break;
		
		default:
		break;
	}
	
	switch(state){ // state actions
		
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
	return state;
}

enum BLINKINGLED_STATES {BL_start,  BL_ThreeOn, BL_ZeroOn};
int TickFct_BlinkingLeds(int state){
	
	switch(state){
		case BL_start:
		state = BL_ThreeOn;
		break;
		
		case BL_ThreeOn:
		state = BL_ZeroOn;
		break;
		
		case  BL_ZeroOn:
		state = BL_ThreeOn;
		break;
		
		default:
		break;
	}
	
	switch(state){	//state actions
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
	return state;
}

enum COMBINELED_STATES {CO_start, CO_combine };
int TickFct_CombineLeds(int state){
	
	switch(state){
		case CO_start:
		state =  CO_combine;
		break;
		
		case CO_combine:
		state = CO_combine;
		break;
		
	}
	
	switch(state){
		case CO_start:
		break;
		
		case CO_combine:
		PORTB = threeLEDs | bilnkingLED;
		break;
		
	}
	
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
	DDRB = 0xFF; PORTB = 0x00; //set PORTB as output initialize to 0
	unsigned char i = 0;
	tasks[i].state = Th_start;
	tasks[i].period = periodThreeLED;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_ThreeLeds;
	i++;
	tasks[i].state = BL_start;
	tasks[i].period = periodBlinkingLED;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_BlinkingLeds;
	i++;
	tasks[i].state = CO_start;
	tasks[i].period = periodCombineLED;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_CombineLeds;
	
	
	TimerSet(taskPeriodGCD);
	TimerOn();
	
	while (1)
	{
	}
}

