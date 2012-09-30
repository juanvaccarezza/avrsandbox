/*
 * main.c
 *
 *  Created on: Sep 24, 2012
 *      Author: root
 */
#include <avr/io.h>
#include <util/delay.h>

#include "stepper.h"

void main()
{
	stepperRegist regist;
	regist.currentPattern = 3;
	regist.patternLength = 4;
	regist.pattern[0] = 0b00001000;
	regist.pattern[1] = 0b00000001;
	regist.pattern[2] = 0b00000100;
	regist.pattern[3] = 0b00000010;

   //Initialize the stepper library
   StepperInit();

   _delay_loop_2(10000);

   while(1)
   {

      for(uint16_t i=0;i<24*40;i++)
      {
         StepperStepCW(&regist);     //Step Clock wise

         _delay_ms(1);
         PORTA &= 0xF0;
         _delay_ms(1);

      }

     for(uint16_t i=0;i<24*5;i++)
     {
        StepperStepCCW(&regist);    //Step Counter Clock wise
        _delay_loop_2(10000);
     }

   }
}

