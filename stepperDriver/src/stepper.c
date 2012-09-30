/*
 * stepper.c
 *
 *  Created on: Sep 24, 2012
 *      Author: root
 */

#include "stepper.h"
#include "stepperconf.h"

#include <avr/io.h>

void StepperInit()
{
	__STEPPER_DDR|=(0X0F<<STEPPER_POSITION);
}


void StepperStepCCW(stepperRegist * registers)
{
	if(registers->currentPattern == 0){
			registers->currentPattern = registers->patternLength -1;
		} else {
			registers->currentPattern--;
		}
		__STEPPER_PORT = ((__STEPPER_PORT & ~(0x0F<<STEPPER_POSITION))| (registers->pattern[registers->currentPattern]<<STEPPER_POSITION));
}

void StepperStepCW(stepperRegist * registers)
{
	if(registers->currentPattern == (registers->patternLength-1)){
		registers->currentPattern = 0;
	} else {
		registers->currentPattern++;
	}
	__STEPPER_PORT = ((__STEPPER_PORT & ~(0x0F<<STEPPER_POSITION))| (registers->pattern[registers->currentPattern]<<STEPPER_POSITION));
}
