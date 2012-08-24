#include "sevenSegment.h"

/*
 * the following on port pins is assumed
 *
 *     ________
 *    |   2    |
 *    |1      3|
 *    |________|
 *    |   0    |
 *    |7      5|
 *    |___6____|  .4
 *
 *
 */
static uint8_t patterns [11] = {
		0b11101110,
		0b00101000,
		0b11001101,
		0b01101101,
		0b00101011,
		0b01100111,
		0b11100111,
		0b00101101,
		0b11101111,
		0b00101111,
		0b00010000,
};

uint8_t getPattern (sevenSegDescriptor * descriptor, uint8_t number){
	return descriptor->commonAnnode?~patterns[number]:patterns[number];
}

void write(sevenSegDescriptor * desc, uint8_t number){
	*(desc->ledPort) = getPattern(desc,number);
}

void enable(sevenSegDescriptor * desc, uint8_t display){
		if(desc->commonAnnode){
			sbi(*(desc->common[display].port),desc->common[display].pinNo);
		} else {
			cbi(*(desc->common[display].port),desc->common[display].pinNo);
		}
}

void disable(sevenSegDescriptor * desc, uint8_t display){
	if(desc->commonAnnode){
		cbi(*(desc->common[display].port),desc->common[display].pinNo);
	} else {
		sbi(*(desc->common[display].port),desc->common[display].pinNo);
	}
}

void enableOnly(sevenSegDescriptor * desc, uint8_t display){
	for (int displayIt = 0; displayIt < desc->amountOfDisplays; ++displayIt) {
		if(display == displayIt){
			enable(desc,displayIt);
		} else {
			disable(desc,displayIt);
		}
	}
}

void enableAndWrite(sevenSegDescriptor * desc, uint8_t display, uint8_t number){
	enableOnly(desc,display);
	write(desc,number);
}

