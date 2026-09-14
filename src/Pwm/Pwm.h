/*
 *  Title: Pwm.h
 *	Version:
 *  Description:
 *  Created:  03/11/2014 NW
 *  Last Mod: xx/xx/xxxx XX
 *  Author: Cátedra "Sistemas de Control"
 *  		Departamento de Electrónica
 *  		Facultad de Ingeniería
 *  		Universidad Nacional de Mar del Plata
 */

#ifndef PWM_H_
#define PWM_H_

extern CLK_Handle myClk;
extern PWM_Handle myPwm1, myPwm2, myPwm3, myPwm4;

void HRPWM1_Config(int);
void HRPWM2_Config(int);
void HRPWM3_Config(int);
void HRPWM4_Config(int);

#endif /* PWM_H_ */
