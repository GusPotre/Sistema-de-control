/*	UNIVERSIDAD TECNOLÓGICA DE PUEBLA
 *	División de Mecatrónica
 *	
 *	Autor: Carlos J. Morales
 *	Descripción: Configuración y control básico
 *		del puerto serie (COM) utilizando el API Win32;
 *		con lectura asíncrona.
 *		Configuración fija:
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
 *					int dataLen [o] : número de datos leídos.
 *				);
 *			- writePort(
 *					char dataTx [i] : caracteres (o cadena) a enviar.
 *					int dataLen [i] : número de caracteres a enviar.
 *				);
 *	Fecha: septiembre 2022.
 *
 *	NOTAS:
 *		1.	Cualquier modificación está permitida, con
 * 			el respectivo crédito al autor principal.
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
