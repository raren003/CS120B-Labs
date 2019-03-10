/*
 * raren003_eepromtesting.c
 *
 * Created: 3/9/2019 6:55:51 PM
 * Author : Rob
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <avr/eeprom.h>
#include "io.c"

unsigned int EEMEM value;

int main(void)
{
	DDRC = 0xFF; PORTC = 0x00;	//LCD data lines
	DDRD = 0xFF; PORTD = 0x00;	//LCD control lines
	
	LCD_init();
    
	unsigned int SRAMint;
	char buffer[20];
	
	eeprom_write_word(&value, 156);
	
	
	itoa (eeprom_read_word(&value),buffer,10);
	LCD_DisplayString(1, buffer);
	
	
	
    while (1) 
    {
    }
}

