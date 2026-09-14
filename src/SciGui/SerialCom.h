/*
 * Serial Communications API
 * 
 * Created: 31/OCT/2012 MH
 * LastMod: 07/NOV/2012 MH
 */

#ifndef SERIALCOM_H_
#define SERIALCOM_H_

void InitSerialCom( void );

// Nuevas funciones
char ReadInputMsgFromSerialComMod(char Message[4], float Modifier[2]);

void WriteOutputMsgToSerialCom(char Message[4], float Modifier[2]);

void scia_xmit(int a);

void ReadInputMsgFromSerialCom(char *pMsgRdyFlag, char *pMsg, const int MaxMsgLen);

// Envía por el SerialCom el array de datos 'long'.
void PushLongArrayToSerialCom(const long *pData, const int DataLen);

// Envía por el SerialCom el array de datos 'float'.
void PushFloatArrayToSerialCom(const float *pData, const int DataLen);

// Considera como 'float' al dato que viene del SerialCom y lo carga en *pDataRx.
// Devuelve 1 si actualizó el dato, 0 si no.
char ReadFloatFromSerialCom(float *pDataRx);

#endif /*SERIALCOM_H_*/
