/*
 * sevenSegment.h
 *
 *  Created on: Aug 23, 2012
 *      Author: root
 */

#ifndef SEVENSEGMENT_H_
#define SEVENSEGMENT_H_

#include <inttypes.h>
#include <stdbool.h>

#define SEVEN_SEG_STOP 10

#define sbi(var, mask)   ((var) |= (uint8_t)(1 << mask))
#define cbi(var, mask)   ((var) &= (uint8_t)~(1 << mask))

typedef struct displayCommon_t{
    uint8_t * port;
	uint8_t pinNo;
} displayCommon;

typedef struct sevenSegDescriptor_t{
	bool commonAnnode;
	uint8_t * ledPort;
	uint8_t amountOfDisplays;
	displayCommon * common;
} sevenSegDescriptor;

uint8_t getPattern (sevenSegDescriptor * descriptor, uint8_t number);

void write(sevenSegDescriptor * desc, uint8_t number);

void enable(sevenSegDescriptor * desc, uint8_t number);

void enableAndWrite(sevenSegDescriptor * desc, uint8_t display, uint8_t number);




#endif /* SEVENSEGMENT_H_ */
