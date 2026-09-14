/*
 * Serial Communications API
 * 
 * Created: 31/OCT/2012 MH
 * LastMod: 31/OCT/2012 MH
 * 
 * Este archivo fue creado a partir de "Example_2833xSci_Echoback.c", del proyecto de ejemplo "DSP2833x SCI Echoback".
 */

// Std includes
#include <string.h>

// Project specific & low-level includes
//#include "PeripheralHeaderIncludes.h"
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

// App includes
#include "SerialCom.h"

// Prototype statements for functions found within this file.
static void scia_echoback_init(void);
static void scia_fifo_init(void);
void scia_xmit(int a);
//static void scia_msg(char *msg);

void
InitSerialCom(void) 
{
	// For this example, only init the pins for the SCI-A port.
	// This function is found in the DSP2833x_Sci.c file.
	InitSciaGpio();


    scia_fifo_init();	   // Initialize the SCI FIFO
    scia_echoback_init();  // Initalize SCI for echoback
}



char ReadInputMsgFromSerialComMod(char Message[4], float Modifier[2])
{
    static int i = 0;
    static char State = 0;
    char RxChar = 0;
    float RxFloat = 0;

    if( (State == 'E') || (State == 'F') ){
    	State = 0;
	} else
	if( SciaRegs.SCIFFRX.bit.RXFFST != 0 )	{	// Verifica si recibió un caracter
		if (State == 0) { // Verifica si el mensaje corresponde a un comando PC2DSP
			RxChar = (char) SciaRegs.SCIRXBUF.all;	// Lee el caracter recibido
			if (RxChar == '@') {
				State = 1;
				i = 0;
			} else {
				State = 'E';
			}
		} else
		if (State == 1) { // Arma el mensaje de entrada
			RxChar = (char) SciaRegs.SCIRXBUF.all;	// Lee el caracter recibido
			if (i<4){
				Message[i] = RxChar;
				if (i==3) { State=2; }
			} else {
				State='E';
			}
			i++;
		} else
		if (State == 2) { // Verifica si el mensaje corresponde a un comando PC2DSP
			RxChar = (char) SciaRegs.SCIRXBUF.all;	// Lee el caracter recibido
			if (RxChar == '#') {
				State = 3;
				i = 0;
			} else {
				State = 'E';
			}
		} else
		if (State == 3) { // Arma el modificador del mensaje de entrada
			if ( ReadFloatFromSerialCom(&RxFloat) == 1 ) { // Lee el numero recibido
				if (i<2) {
					Modifier[i]=RxFloat;
					if (i==1) { State=4; }
				} else {
					State = 'E';
				}
				i++;
			}
		} else
		if (State == 4) { // Verifica si el mensaje corresponde a un comando PC2DSP
			RxChar = (char) SciaRegs.SCIRXBUF.all;	// Lee el caracter recibido
			if ((RxChar == 0)||(RxChar == '!')) {
				State = 'F';
			} else {
				State = 'E';
			}
		} else {
			State = 0;
		}
	}
	return( State );
}



void WriteOutputMsgToSerialCom(char Message[4], float Modifier[2])
{
	scia_xmit('@');
	scia_xmit(Message[0]);
	scia_xmit(Message[1]);
	scia_xmit(Message[2]);
	scia_xmit(Message[3]);
	scia_xmit('#');
	PushFloatArrayToSerialCom(Modifier, 2);
	scia_xmit('!');
}




void
ReadInputMsgFromSerialCom(char *pMsgRdyFlag, char *pMsg, const int MaxMsgLen)
{
    static int i = 0;
    char RxChar = 0;
	
    *pMsgRdyFlag = 0;	// Por default devuelve 0 (=sin mensaje).

	if( SciaRegs.SCIFFRX.bit.RXFFST != 0 )		// Verifica si recibió un caracter
	{
		// Arma el mensaje de entrada
       	RxChar = (char) SciaRegs.SCIRXBUF.all;	// Lee el caracter recibido
       	pMsg[i++] = RxChar;						// Guarda el caracter en el texto de entrada

       	if( RxChar == '\0' ){					// Detecta si hay fin de mensaje
       		if( i > 1 ) {						// Verifica que el mensaje sea de longitud no nula
       			*pMsgRdyFlag = 1;				// Avisa q hay mensaje listo
       		}
       		i = 0;	// Para comenzar un nuevo mensaje en la sig iteración
       	} else {
       		// Controla que no se vaya de rango el puntero de la cadena.
       		// Si se excede, sigue llenando la cadena desde el ppio (cíclico).
       		if( i >= MaxMsgLen ){
       			i = 0;
       		}
       	}
	}
}

void
PushFloatArrayToSerialCom(const float *pData, const int DataLen)
{
	int i = 0, j = 0;
    Uint16 TxChar = 0;
    Uint32 TxWord = 0;
	
	if( DataLen < 1 ) {
		return;
	}

	// Envía los datos de a uno a la vez por el puerto serie	
	for(i = 0; i < DataLen; i++ )
	{
	   	// Envía el número flotante de a 8 bits a la vez
	   	memcpy ( &TxWord, &pData[i], sizeof(&pData[i]) );
	   	for( j = 0; j < 4; j++ ) {
	   		TxChar = (TxWord >> (j*8)) & (0x000000FF);
	       	scia_xmit(TxChar);
	   	}
	}
}


void
PushLongArrayToSerialCom(const long *pData, const int DataLen)
{
	int i = 0, j = 0;
    Uint16 TxChar = 0;
    Uint32 TxWord = 0;

	if( DataLen < 1 ) {
		return;
	}

	// Envía los datos de a uno a la vez por el puerto serie
	for(i = 0; i < DataLen; i++ )
	{
	   	// Envía el número flotante de a 8 bits a la vez
	   	memcpy ( &TxWord, &pData[i], sizeof(&pData[i]) );
	   	for( j = 0; j < 4; j++ ) {
	   		TxChar = (TxWord >> (j*8)) & (0x000000FF);
	       	scia_xmit(TxChar);
	   	}
	}
}


/*
void
PushInt32ToSerialCom(const float *pData, const int DataLen)
{
	int i = 0, j = 0;
    Uint16 TxChar = 0;
    Uint32 TxWord = 0;

	if( DataLen < 1 ) {
		return;
	}

	// Envía los datos de a uno a la vez por el puerto serie
	for(i = 0; i < DataLen; i++ )
	{
	   	// Envía el número flotante de a 8 bits a la vez
	   	memcpy ( &TxWord, &pData[i], sizeof(&pData[i]) );
	   	for( j = 0; j < 4; j++ ) {
	   		TxChar = (TxWord >> (j*8)) & (0x000000FF);
	       	scia_xmit(TxChar);
	   	}
	}
}
*/

char ReadFloatFromSerialCom(float *pDataRx)
{
	static int i = 0;
    static Uint32 RxWord = 0;
    Uint16 RxChar = 0;
    int ReturnValue = 0;
    
	if( SciaRegs.SCIFFRX.bit.RXFFST != 0 )	// Verifica si hay dato en el SerialCom
	{
       	// Reconstruye el número asumiendo LSB 1st
       	RxChar = SciaRegs.SCIRXBUF.all;
       	RxWord |= ((Uint32) RxChar) << (i*8);
       	if( ++i == 4 ) {	// Cuando leyó 4 bytes, actualiza el dato con un memcpy.
       		memcpy ( pDataRx, &RxWord, sizeof(RxWord) );
       		i = 0;
       		RxWord = 0;
       		ReturnValue = 1;
       	}
	}
	
	return( ReturnValue );
}

// Test 1,SCIA  DLB, 8-bit word, baud rate 0x000F, default, 1 STOP bit, no parity
void scia_echoback_init()
{
    // Note: Clocks were turned on to the SCIA peripheral
    // in the InitSysCtrl() function

 	SciaRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback
                                   // No parity,8 char bits,
                                   // async mode, idle-line protocol
	SciaRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK,
                                   // Disable RX ERR, SLEEP, TXWAKE
	SciaRegs.SCICTL2.all =0x0003;
	SciaRegs.SCICTL2.bit.TXINTENA =1;
	SciaRegs.SCICTL2.bit.RXBKINTENA =1;
	#if (CPU_FRQ_150MHZ)
//	      SciaRegs.SCIHBAUD    =0x0001;  // 9600 baud @LSPCLK = 37.5MHz.
//	      SciaRegs.SCILBAUD    =0x00E7;
	      SciaRegs.SCIHBAUD    =0x0000;  // 115200 baud @LSPCLK = 37.5MHz.
	      SciaRegs.SCILBAUD    =0x0028;
//	      SciaRegs.SCIHBAUD    =0x0000;  // 460800 baud @LSPCLK = 37.5MHz.
//	      SciaRegs.SCILBAUD    =0x000A;
	#endif
	#if (CPU_FRQ_100MHZ)
      SciaRegs.SCIHBAUD    =0x0001;  // 9600 baud @LSPCLK = 20MHz.
      SciaRegs.SCILBAUD    =0x0044;
	#endif
	SciaRegs.SCICTL1.all =0x0023;  // Relinquish SCI from Reset
}

// Transmit a character from the SCI
void scia_xmit(int a)
{
    while (SciaRegs.SCIFFTX.bit.TXFFST != 0) {}
    SciaRegs.SCITXBUF = a;
}

/*
void scia_msg(char *msg)
{
    int i;
    i = 0;
    while(msg[i] != '\0')
    {
        scia_xmit(msg[i]);
        i++;
    }
}
*/

// Initalize the SCI FIFO
void scia_fifo_init()
{
    SciaRegs.SCIFFTX.all=0xE040;
    SciaRegs.SCIFFRX.all=0x204f;
    SciaRegs.SCIFFCT.all=0x0;
}
