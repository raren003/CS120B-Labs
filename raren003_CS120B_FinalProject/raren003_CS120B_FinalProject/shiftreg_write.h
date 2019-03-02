/* Partner(s) Name & E-mail: Robert Arenas, raren003@ucr.edu
* Lab Section: 022
* Assignment: Final Project
* Exercise Description: Header file that contains code that controls output
*						to the shift register
*
* I acknowledge all content contained herein, excluding template or example
* code, is my own original work.
*/


#ifndef SHIFTREG_WRITE_H_
#define SHIFTREG_WRITE_H_

//port the shift register is connected to
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



#endif /* SHIFTREG_WRITE_H_ */