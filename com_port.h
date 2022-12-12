/*	UNIVERSIDAD TECNOL�GICA DE PUEBLA
 *	Divisi�n de Mecatr�nica
 *	
 *	Autor: Carlos J. Morales
 *	Descripci�n: Configuraci�n y control b�sico
 *		del puerto serie (COM) utilizando el API Win32;
 *		con lectura as�ncrona.
 *		Configuraci�n fija:
 *			- 8 bits
 *			- 9600 baudios
 *			- 1 bit de parada
 *			- Sin paridad
 *		Funciones:
 *			- openPort(
 *					char comPort [i] : nombre del puerto.
 *				);
 *			- closePort(
 *					// Sin argumentos.
 *				);
 *			- readPort(
 *					char dataRx [i] : vector de caracteres para guardar
 *										los datos recibidos.
 *					int dataLen [o] : n�mero de datos le�dos.
 *				);
 *			- writePort(
 *					char dataTx [i] : caracteres (o cadena) a enviar.
 *					int dataLen [i] : n�mero de caracteres a enviar.
 *				);
 *	Fecha: septiembre 2022.
 *
 *	NOTAS:
 *		1.	Cualquier modificaci�n est� permitida, con
 * 			el respectivo cr�dito al autor principal.
 *		2.	Cualquier error puede ser reportado a 
 *			carlos.morales@utpuebla.edu.mx
 */

#ifndef COM_PORT
#define COM_PORT

extern HANDLE	hComm;
extern DCB 		dcbPort;
extern BOOL		portStt;
extern char 	portName[8];
    
void openPort(char comPort[]);
void closePort(void);
void flushBuffer(void);
void readPort(char dataRx[], int *dataLen);
void writePort(char dataTx[], int dataLen);

#endif
