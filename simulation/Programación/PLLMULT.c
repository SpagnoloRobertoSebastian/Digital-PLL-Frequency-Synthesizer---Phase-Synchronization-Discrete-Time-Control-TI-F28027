#include <18F4550.h>
#include <math.h>
#fuses XT            //Reloj interno <= 4MHz
#fuses NOMCLR        //Pin de entrada RE3 habilitado
#fuses PUT           //Power up Timer: es un Timer que permite hacer reset por software

//Uso de librerias
#use delay (clock=4000000)
#use fast_io(d)                  //Llamo la libreria del puerto D

void main (void)
{ //Hay que configurar los puertos
   int leds=0b11111111;          //Delcaro la variable de 1 byte con su respectivos valores
   set_tris_d(0b00000000);       //Configuro los bit del puerto D como salida
   
   //declarando variables
   float wl, Vref, VO, dw_k, w0, phi0, p, pm_k;
    wL=6.2831853*50;        
    dw_k=0;
   
   // historia del filtro
   static float p_k;                // Entrada al filtro en t= k
   static float p_k_1;              // Entrada al filtro en t= k-1
   static float pm_k_1;            // salida del filtro en t= k o p media

   // historia del compensador
   static float em_k;                     // Entrada al compensador en t= k o la media del error
   static float em_k_1;                  // Entrada al compensador en t= k-1 
   static float dw_k_1;                  // salida del compensador en t= k-1

   while(true)
   {
      
    w0=dw_k+wL;
    
    //integro la fase
    phi0 += w0;    
   if( phi0 > 6.2831853)
   {
      phi0 -= 6.2831853
   }
    
    V0=sin(phi0)
    p_k = Vref*V0        //salida del multiplicador
   
   // Aplicar filtrado al producto de las señales
   //    y(k)= 0.0618551 x(k-1) + 0.9690724 y(k-1)

   pm_k = 0.0618551 * p_k + 0.9690724 * pm_k_1;


   // Aplicar el compensador
   // y(k)=  63.6619772 x(k) - 63.4619772  x(k-1) + y(k-1)

   dw_k = 63.6619772 * em_k - 63.4619772 * em_k_1 + dw_k_1;
   
   }
   
   // ActualizaciÃ³n de la historia de las variables
   p_k_1      = p_k;
   dw_k_1 = dw_k;
   em_k_1 = em_k;

}
