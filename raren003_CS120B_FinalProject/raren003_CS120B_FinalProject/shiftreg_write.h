/*
 * shiftreg_write.h
 *
 * Created: 3/2/2019 10:20:16 AM
 *  Author: Rob
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