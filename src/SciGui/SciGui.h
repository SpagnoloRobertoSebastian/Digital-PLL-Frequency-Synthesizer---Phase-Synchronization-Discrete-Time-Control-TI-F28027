/*
 * Virtual SerialCom-GUI API
 * 
 * Una GUI virtual basada en puerto serie (usa el módulo SerialCom).
 * Aplicación Host implementada con scripts de MatLab.
 * 
 * Created: 07/NOV/2012 MH
 * LastMod: 07/NOV/2012 MH
 */

#ifndef SCIGUI_H_
#define SCIGUI_H_

#define MAX_COMM_VECTOR_LENGTH	( 200 )

extern float CommVector0[MAX_COMM_VECTOR_LENGTH];
extern float CommVector1[MAX_COMM_VECTOR_LENGTH];

extern int datacomm_enable;
extern int datacomm_enable_flag;

extern float CommVectorLatch [MAX_COMM_VECTOR_LENGTH];

interrupt void sciaRxFifoIsr(void);

void SendTemperatureToPC(float);

void InitSciGui(void);
void PumpSciGui(void);

extern void PushFloatToCommVector0(float data);
extern void PushFloatToCommVector1(float data);

extern int referencia;

extern void datacomm_function(void);

extern float AccTempToPC;
extern float AccTemp1ToPC;
extern float AccTemp2ToPC;

extern float DataToPc1;
extern float DataToPc2;

// GPIO 0
#define TURN_ON_LED()		{GpioDataRegs.GPACLEAR.bit.GPIO1 = 1;}
#define TURN_OFF_LED()		{GpioDataRegs.GPASET.bit.GPIO1 = 1;}
#define TOGGLE_LED()	    {GpioDataRegs.GPATOGGLE.bit.GPIO1 = 1;}

#endif /*SCIGUI_H_*/
