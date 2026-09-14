/*
 *  Title: control.c
 *   Version:
 *  Description:
 *  Created:  16/07/2014 NW
 *  Last Mod: xx/xx/xxxx XX
 *  Author: Cátedra "Sistemas de Control"
 *        Departamento de Electrónica
 *        Facultad de Ingeniería
 *        Universidad Nacional de Mar del Plata
 *
 *   En este archivo se implementa la función PLL_SdC_function() en la cual debe ser incluida la implementación del PLL.
 *   Entradas:
 *       Input10V: 0-10V
 *   Salidas:
 *      *UserLED / *UserGPIO: 0(0V):OFF-1(3.3V):ON
 *      Output3_3V: 0-3.3V

 */

#include <math.h>
int Output_Selector=0;

float PLL_SdC_function(float Input10V, int *UserLED, int *UserGPIO) // return Output3_3V
{
float Output3_3V=1.65;
float Referencia;                     // Mapea entrada  a +-1V
float wL =314.15927;               //frecuencia de linea desada 2*pi*50
float pma = 0;                           // definicion del punto de operacion pma
float w0 = 0;
float Ts=0.001;                        //Tiempo de muestreo
//const IncrementoLinealDeFase = 0.31416;   // EscalÃ³n normal de la rampoa de fase 2pi / 20

// historia del filtro
static float P_k_1=0;               // Producto Entrada al filtro en t= k-1
static float P_k;               // Entrada al filtro en t= k
static float Pm_k_1=0;            //  salida del filtro en t= k-1
static float Pm_k;               //  salida del filtro en t= k

// historia del compensador
static float Em_k_1=0;            // Entrada al compensador en t= k
static float Em_k;               // Entrada al compensador en t= k
static float Dw_k;                  // salida del compensador en t= k
static float Dw_k_1=0;           // salida del compensador en t= k-1

static float fase0=0;            // fase de salida
static float SenialGenerada=0;               //salida v0
float Salida;


   // Input conditioning
   Referencia = (2.0*Input10V/5.0)-1;   //muesta de la entrada en t=k

   // --**--**-- Inicio del codigo del alumno --**--**--


   P_k = Referencia * SenialGenerada; //

   // Aplicar filtrado al producto de las seÃ±ales
   //    y(k)= 0.0618551 x(k-1) + 0.9690724 y(k-1)

   Pm_k = 0.0579437 * P_k_1 + 0.9420563* Pm_k_1;

   // Calculo del error= pm*-pm(k)
   Em_k = pma - Pm_k;

   // Aplicar el compensador
   // y(k)=  63.6619772 x(k) - 63.4619772  x(k-1) + y(k-1)

   Dw_k = -118.66794 * Em_k_1 +119.38052 * Em_k + Dw_k_1;

   // calculo de la fase de salida en el instante k
   w0= Dw_k + wL;
   fase0= fase0+ (w0*Ts);
    if( fase0 > 6.2831853)
      fase0 -= 6.2831853;

   // Generar la señal de salida vo en el instante k
   SenialGenerada = sin(fase0);

   // ActualizaciÃ³n de la historia de las variables
   P_k_1     = P_k;
   Pm_k_1    = Pm_k;
   Em_k_1    = Em_k;
   Dw_k_1    = Dw_k;

   switch(Output_Selector)
   {
   case(5): Output3_3V= (SenialGenerada+1.0)*3.3/2.0; break;
   case(1): Output3_3V= (Dw_k+25.0)*3.3/50.0; break;		// Salida del compensador
   case(2): Output3_3V= (Em_k+1.0)*3.3/2.0; break;		// error
   case(3): Output3_3V= (P_k+1.0)*3.3/2.0; break;		// Producto
   case(4): Output3_3V= (Pm_k+1.0)*3.3/2.0; break;		// Salida del filtro
   }
   // --**--**-- Fin del codigo del alumno    --**--**--


   // Output conditioning
   //Output3_3V= (Salida+1.0)*3.3/2.0;

   return(Output3_3V);
}

