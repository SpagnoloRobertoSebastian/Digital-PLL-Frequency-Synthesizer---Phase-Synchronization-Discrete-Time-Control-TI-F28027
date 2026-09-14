/*
 *  Title: control.c
 *	Version:
 *  Description:
 *  Created:  16/07/2014 NW
 *  Last Mod: xx/xx/xxxx XX
 *  Author: Cátedra "Sistemas de Control"
 *  		Departamento de Electrónica
 *  		Facultad de Ingeniería
 *  		Universidad Nacional de Mar del Plata
 *
 *	En este archivo se implementa la función PLL_SdC_function() en la cual debe ser incluida la implementación del PLL.
 *	Entradas:
 * 		Input10V: 0-10V
 *	Salidas:
 *		*UserLED / *UserGPIO: 0(0V):OFF-1(3.3V):ON
 *		Output3_3V: 0-3.3V

 */

#include <math.h>
int Output_Selector=0;

float PLL_SdC_function(float Input10V, int *UserLED, int *UserGPIO) // return Output3_3V
	{
	float Output3_3V=1.65;
	float InputPM1V, OutputPM1V; // Mapea entrada y salida a +-1V

	// Input conditioning
	InputPM1V = (Input10V/5.0)-1;

	// --**--**-- Inicio del código del alumno --**--**--
	OutputPM1V=InputPM1V;
	// --**--**-- Fin del código del alumno    --**--**--

	// --**--**-- Inicio del código de ejemplo 1 --**--**--
	/*
		// Matlab 50Hz Low-Pass Filter
				// >> s=tf('s');
				// >> c2d(1/(s/(2*pi*50)+1),1e-3)
				// Transfer function:
				//   0.2696
				// ----------
				// z - 0.7304
				// Sampling time: 0.001
			float NumCoefZ1=0.2696;
			float DenCoefZ1=0.7304;
			float OutputZ0;
			static float OutZ1,InpZ1;
			OutputZ0= NumCoefZ1 * InpZ1 + DenCoefZ1 * OutZ1;
			OutZ1=OutputZ0;
			InpZ1=InputPM1V;
		// Saturation and Flag
			float OutputSat;
			*UserGPIO=1;
			if      (OutputZ0>0.25)  OutputSat=0.25;
			else if (OutputZ0<-0.25) OutputSat=-0.25;
			else
			{
				OutputSat=OutputZ0;
				*UserGPIO=0;
			}
		// Output Selector
			switch ( Output_Selector ) {
			case 0:
				OutputPM1V=InputPM1V;
			  break;
			case 1:
				OutputPM1V=OutputZ0;
			  break;
			case 2:
				OutputPM1V=OutputSat;
			  break;
			default:
				OutputPM1V=0;
			  break;
			}
	*/
	// --**--**-- Fin del código de ejemplo 1 --**--**--

	// Output conditioning
	Output3_3V= (OutputPM1V+1.0)*3.3/2.0;

	return(Output3_3V);
	}
