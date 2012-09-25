/*
 * stepperconf.h
 *
 *  Created on: Sep 24, 2012
 *      Author: root
 */

#ifndef STEPPERCONF_H_
#define STEPPERCONF_H_

#define STEPPER_PORT A
#define STEPPER_POSITION 0

#define _CONCAT(a,b) a##b
#define PORT(x) _CONCAT(PORT,x)
#define PIN(x) _CONCAT(PIN,x)
#define DDR(x) _CONCAT(DDR,x)

#define __STEPPER_PORT 	PORT(STEPPER_PORT)
#define __STEPPER_DDR	DDR(STEPPER_PORT)



#endif /* STEPPERCONF_H_ */
