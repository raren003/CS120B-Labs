/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu, Noah Marestaing, nmare001@ucr.edu
* Lab Section: 022
* Assignment: Lab #11 Exercise #3
* Exercise Description: [optional - include for your own benefit]
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/

#include <avr/io.h>
#include <avr/common.h>
#include <avr/interrupt.h>
#include "scheduler.h"
#include "keypad.h"
#include "io.c"

/*
typedef struct task {
	int state;
	unsigned long period;
	unsigned long elapsedTime;
	int (*TickFct)(int);
} task; */

task tasks[2];
const unsigned short tasksNum = 2;
const unsigned long taskPeriodGCD = 2;
const unsigned long periodKeypad = 2;
const unsigned long periodLCD = 500;



static unsigned char x; // used to get input from keypad
static unsigned char* lcdOutput = " "; // used to display output on LCD

enum KEYPAD_STATES {KEY_start,  KEY_Input};
int TickFct_Keypad(int state){
	
	switch(state){
		case KEY_start:
		state = KEY_Input;
		break;
		
		case KEY_Input:
		state = KEY_Input;
		break;
		
		default:
		break;
	}
	
	switch(state){
		case KEY_start:
		break;
		
		case KEY_Input:
			x = GetKeypadKey();
			switch(x) {
				case '\0': PORTB = 0x1F; break;		//All 5 LEDs on
				case '1': PORTB = 0x01; lcdOutput = "1"; break;		//hex equivalent
				case '2': PORTB = 0x02; lcdOutput = "2"; break;
				
				
				//  . . . ?***** FINISH *****
				case '3': PORTB = 0x03; lcdOutput = "3"; break;
				case '4': PORTB = 0x04; lcdOutput = "4"; break;
				case '5': PORTB = 0x05; lcdOutput = "5"; break;
				case '6': PORTB = 0x06; lcdOutput = "6"; break;
				case '7': PORTB = 0x07; lcdOutput = "7"; break;
				case '8': PORTB = 0x08; lcdOutput = "8"; break;
				case '9': PORTB = 0x09; lcdOutput = "9"; break;
				case 'A': PORTB = 0x0A; lcdOutput = "A"; break;
				case 'B': PORTB = 0x0B; lcdOutput = "B"; break;
				case 'C': PORTB = 0x0C; lcdOutput = "C"; break;
				
				case 'D': PORTB = 0x0D; lcdOutput = "D"; break;
				case '*': PORTB = 0x0E; lcdOutput = "*"; break;
				case '0': PORTB = 0x00; lcdOutput = "0"; break;
				case '#': PORTB = 0x0F; lcdOutput = "#"; break;
				default: PORTB = 0x1B;  lcdOutput = "R"; break;	// Should never occur. Middle LED OFF
			}
		break;
		
		default:
		break;
	}
	
	return state;
}


enum LCD_STATES {LCD_start,  LCD_Display};
int TickFct_LCD(int state){
	
	switch(state){		//transitions
		case LCD_start:
			state = LCD_Display;
		break;
		
		case LCD_Display:
			state = LCD_Display;
		break;
		
		default:
		break;
	}					//transitions
	
	
	switch(state){		//state actions
		case LCD_start:
		break;
		
		case LCD_Display:
			LCD_DisplayString(1, lcdOutput);
		break;
		
		default:
		break;
	}					//state actions
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
    DDRB = 0xFF; PORTB = 0x00; // PORTB set to output, outputs init 0s
    DDRA = 0xF0; PORTA = 0x0F; // PC7..4 outputs init 0s, PC3..0 inputs init 1s
	
	DDRC = 0xFF; PORTC = 0x00;	//LCD data lines
	DDRD = 0xFF; PORTD = 0x00;	//LCD control lines
	
	LCD_init();
	
	unsigned char i = 0;
	tasks[i].state = KEY_start;
	tasks[i].period = periodKeypad;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_Keypad;
	i++;
	tasks[i].state = LCD_start;
	tasks[i].period = periodLCD;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_LCD;
	
	TimerSet(taskPeriodGCD);
	TimerOn();
	
    while (1) 
    {
    }
}