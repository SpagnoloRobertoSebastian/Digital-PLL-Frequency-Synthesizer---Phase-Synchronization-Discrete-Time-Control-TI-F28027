/*
 *  Title: config.c
 *	Version:
 *  Description:
 *  Created:  15/07/2014 NW
 *  Last Mod: xx/xx/xxxx XX
 *  Author: Cátedra "Sistemas de Control"
 *  		Departamento de Electrónica
 *  		Facultad de Ingeniería
 *  		Universidad Nacional de Mar del Plata
 */

#ifndef CONFIG_H_
#define CONFIG_H_

	//--- UNCOMMENT ONLY THE BOARD IN USE
		#define Placa1
		// #define Placa2
		// #define Placa3

	//--- UNCOMMENT ONLY THE MODE IN USE
		// #define ModeTest
		#define ModeNormal

	#ifdef Placa1
		#define ADC1_SLOPE  3.3*40.0/4096.0
		#define ADC1_OFFSET 2.1
		#define	ADC2_2V   740.0
		#define	ADC2_10V 3710.0
		#define	ADC3_2V   645.0
 		#define	ADC3_10V 3610.0
	#endif

	#ifdef Placa2
		#define ADC1_SLOPE  3.3*40.0/4096.0
		#define ADC1_OFFSET -1.9
		#define	ADC2_2V   745.0
		#define	ADC2_10V 3710.0
		#define	ADC3_2V   680.0
		#define	ADC3_10V 3660.0
	#endif

	#ifdef Placa3
		#define ADC1_SLOPE  3.3*40.0/4096.0
		#define ADC1_OFFSET 0
		#define	ADC2_2V   740.0
		#define	ADC2_10V 3745.0
		#define	ADC3_2V   660.0
		#define	ADC3_10V 3657.0
	#endif

	#define ADC2_SLOPE  8.0/(ADC2_10V-ADC2_2V)
	#define ADC2_OFFSET (10.0-ADC2_SLOPE*ADC2_10V)
	#define ADC3_SLOPE  8.0/(ADC3_10V-ADC3_2V)
	#define ADC3_OFFSET (10.0-ADC3_SLOPE*ADC3_10V)

#endif /* CONFIG_H_ */
