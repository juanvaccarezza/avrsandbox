/*
 * stepper.h
 *
 *  Created on: Sep 24, 2012
 *      Author: root
 */

#ifndef STEPPER_H_
#define STEPPER_H_
#include <inttypes.h>

typedef struct stepperRegist_t{
	uint8_t patternLength;
	uint8_t pattern [8];
	uint8_t currentPattern;
} stepperRegist;

void StepperInit();
void StepperStepCW(stepperRegist* regist);
void StepperStepCCW();
#endif /* STEPPER_H_ */
