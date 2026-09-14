/*
 * Virtual GUI API
 * 
 * Una GUI virtual basada en puerto serie (usa el módulo SerialCom).
 * Aplicación Host implementada con scripts de MatLab.
 * 
 * Funcionamiento:
 * Lee mensajes (strings) recibidos a través del SerialCom en forma periódica (por polling),
 * y los procesa inmediatamente a medida que llegan (FIFO de 1 posición).
 * 
 * Created: 07/NOV/2012 MH
 * LastMod: 07/NOV/2012 MH
 */

// Std includes
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

// Project specific & low-level includes
//#include "PeripheralHeaderIncludes.h"
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File







// App includes
//#include "../SysInit/SysInit.h"
#include "SciGui.h"
#include "SerialCom.h"
//#include "../Pwm/Pwm.h"
//#include "../Adc/Adc.h"

#include "f2802x_common/include/pie.h"
#include "f2802x_common/include/sci.h"
#include "f2802x_common/include/gpio.h"
#include "f2802x_common/include/adc.h"
#include "f2802x_common/include/timer.h"
#include "IQmathLib.h"
#include "../Control/Control.h"
#include "../config.h"


// Longitud máxima de mensaje en caracteres:
#define MAX_MSG_LEN				( 35 )
#define MAX_TX_DATA_VALUES		( 3 )
#define MAX_RX_DATA_VALUES		( 3 )

//#pragma DATA_SECTION(CommVector0, "DMARAML6")
//#pragma DATA_SECTION(CommVector1, "DMARAML6")
float CommVector0[MAX_COMM_VECTOR_LENGTH];
float CommVector1[MAX_COMM_VECTOR_LENGTH];

float DataTx [MAX_TX_DATA_VALUES];
float DataRx [MAX_RX_DATA_VALUES];

//#pragma DATA_SECTION(CommVectorLatch0, "DMARAML6")
//#pragma DATA_SECTION(CommVectorLatch1, "DMARAML6")
float CommVectorLatch0 [MAX_COMM_VECTOR_LENGTH];
float CommVectorLatch1 [MAX_COMM_VECTOR_LENGTH];

float TempToPC;
float AccTempToPC=0;
float AccTemp1ToPC=0;
float AccTemp2ToPC=0;

float DataToPc1=0;
float DataToPc2=0;

int CommVectorIndex=0;
int CommVectorLength=MAX_COMM_VECTOR_LENGTH;
int CommVectorIndex0=0;
int CommVectorLength0=MAX_COMM_VECTOR_LENGTH;
int CommVectorIndex1=0;
int CommVectorLength1=MAX_COMM_VECTOR_LENGTH;
int CommVectorLength;

int datacomm_enable=0;
int datacomm_enable_flag=0;

void datacomm_function(void);

extern PIE_Handle myPie;
extern SCI_Handle mySci;
extern GPIO_Handle myGpio;
extern ADC_Handle myAdc;
extern TIMER_Handle myTimer0;
extern uint32_t TimerPeriod;
extern int16_t currentTemp;
extern int16_t sampleTemperature(void);
extern int ControlPeriod;
extern int FlagPeriod;
extern int Adq0Period;
extern int Adq1Period;

char RequestState=0;
int com_enable=0;

char  Message[4];
float Modifier[2];

int referencia=0;

float DataFloatToPC[2]={0.0,0.0};

void
InitSciGui( void )
{
	InitSerialCom();
}


void PushFloatToCommVector0(float data){
	if (CommVectorIndex0>=CommVectorLength0){
		CommVectorIndex0=0;}
	else {
		CommVectorIndex0++;}
	CommVector0[CommVectorIndex0]=data;
	}

void PushFloatToCommVector1(float data){
	if (CommVectorIndex1>=CommVectorLength1){
		CommVectorIndex1=0;}
	else {
		CommVectorIndex1++;}
	CommVector1[CommVectorIndex1]=data;
	}


void SendTemperatureToPC(float TemperatureToPC){
TempToPC=TemperatureToPC;
RequestState=2;
}


// --------------------------------------------
void datacomm_function(void)
{

	static float counter_aux=0.0;
	if (datacomm_enable==1){
		/*
		#ifdef ModeTest
			counter_aux++;
			DataToPc2=counter_aux;
		#endif
		*/
		DataFloatToPC[0]=DataToPc1;
		DataFloatToPC[1]=DataToPc2;
		RequestState=2;
	}
}



void
PumpSciGui( void )
{
	if( RequestState==2 ){
		//if (com_enable==1){
			// Modifier[0]=TempToPC;
			// Modifier[1]=77;
			// WriteOutputMsgToSerialCom("Tmes",Modifier);
			// RequestState=0;

			//-------
			WriteOutputMsgToSerialCom("Comm",DataFloatToPC);
			RequestState=0;
		//}
	}
}



interrupt void sciaRxFifoIsr(void)
{
	char MessageState = 0;
	MessageState = ReadInputMsgFromSerialComMod(Message, Modifier);
	if( MessageState=='F' ) {
		if ( strncmp(Message,"Lonn",4)==0 ){ // Led On
			TURN_ON_LED()
		}
		else if ( strncmp(Message,"Loff",4)==0 ){ // Led Off
			TURN_OFF_LED()
		}
		else if ( strncmp(Message,"Ltog",4)==0 ){ // Toggle Led
			TOGGLE_LED()
		}
		if ( strncmp(Message,"19on",4)==0 ){ // Led On
			//
			GPIO_setHigh(myGpio, GPIO_Number_19);
		}
		else if ( strncmp(Message,"19of",4)==0 ){ // Led Off
			//
			GPIO_setLow(myGpio, GPIO_Number_19);
		}
		if ( strncmp(Message,"07on",4)==0 ){ // Led On
			//
			GPIO_setHigh(myGpio, GPIO_Number_7);
		}
		else if ( strncmp(Message,"07of",4)==0 ){ // Led Off
			//
			GPIO_setLow(myGpio, GPIO_Number_7);
		}
		if ( strncmp(Message,"07to",4)==0 ){ // Led On
			//
			GPIO_toggle(myGpio, GPIO_Number_7);
		}
		else if ( strncmp(Message,"Tdsp",4)==0 ) { // Envio de frecuencias de operación del sistema
			currentTemp = ADC_getTemperatureC(myAdc, sampleTemperature());
			DataTx[0]=currentTemp;
			DataTx[1]=456;
			scia_xmit('@');
			PushFloatArrayToSerialCom(DataTx, 2);
			scia_xmit('#');
			scia_xmit(0);
		}
		else if ( strncmp(Message,"Ttim",4)==0 ) { // Timer0 frequency selection
			TimerPeriod = (int)(Modifier[0]);
			TIMER_setPeriod(myTimer0, 60 * TimerPeriod);
		}
		else if ( strncmp(Message,"Tcaf",4)==0 ) { // Control and Flag frequencies selection
			ControlPeriod = (int)(Modifier[0]);
			FlagPeriod    = (int)(Modifier[1]);
		}
		else if ( strncmp(Message,"Tadq",4)==0 ) { // Adquisition frequencies selection
			Adq0Period = (int)(Modifier[0]);
			Adq1Period = (int)(Modifier[1]);
		}
		else if ( strncmp(Message,"Tref",4)==0 ) { // Timer0 frequency selection
			referencia = (int)(Modifier[0]);
		}
		else if ( strncmp(Message,"Cena",4)==0 ) { // Habilitación de envio de datos DSP->PC
			com_enable = (int)(Modifier[0]);
		}
	}
    // Clear Overflow flag
    SCI_clearRxFifoOvf(mySci);
    // Clear Interrupt flag
    SCI_clearRxFifoInt(mySci);
    // Issue PIE ack
    PIE_clearInt(myPie, PIE_GroupNumber_9);
    return;
}
