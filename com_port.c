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
 *	Fecha: 	septiembre 2022.	(v1)
 *			octubre 2022.		(v2)	[Control de flujo de datos y timeouts]
 *
 *	NOTAS:
 *		1.	Cualquier modificación está permitida, con
 * 			el respectivo crédito al autor principal.
 *		2.	Cualquier error puede ser reportado a 
 *			carlos.morales@utpuebla.edu.mx
 */

#include <stdio.h>
#include <windows.h>
#include "com_port.h"

HANDLE	hComm;
DCB 	dcbPort;
BOOL 	portStt;
char 	portName[8];

void openPort(char comPort[]){
	
	char portID[10];
	
	sprintf(portID, "\\\\.\\%s", comPort);
	
	hComm = CreateFile(
		portID,
		GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
    	0,
    	NULL
	);
	
	if(hComm == INVALID_HANDLE_VALUE){
		printf("Puerto inaccesible...\n");
	} else{
		dcbPort.DCBlength = sizeof(dcbPort);
		
		portStt = GetCommState(hComm, &dcbPort);
		if(portStt){
			dcbPort.BaudRate = CBR_9600;
			dcbPort.ByteSize = 8;
			dcbPort.StopBits = ONESTOPBIT;
			dcbPort.fParity	 = FALSE;
			dcbPort.Parity	 = NOPARITY;
			dcbPort.fBinary  = TRUE;
			
			dcbPort.fRtsControl = RTS_CONTROL_ENABLE;
			dcbPort.fDtrControl = DTR_CONTROL_ENABLE;
		
			COMMTIMEOUTS timeouts = {0};
  			timeouts.ReadIntervalTimeout = 0;
  			timeouts.ReadTotalTimeoutConstant = 100;
  			timeouts.ReadTotalTimeoutMultiplier = 0;
  			timeouts.WriteTotalTimeoutConstant = 100;
  			timeouts.WriteTotalTimeoutMultiplier = 0;
 
 			SetCommTimeouts(hComm, &timeouts);
			
			portStt = SetCommState(hComm, &dcbPort);
			if(portStt){
				strcpy(portName, comPort);
						
				printf("\nPuerto %s abierto correctamente...\n\n", portName);
			} else{
				printf("\nError al abrir el puerto %s...\n\n", portName);
			}
		} else{
			printf("\nError al configurar el puerto %s...\n\n", portName);
		}	
	}
}

void readPort(char dataRx[], int *dataLen){
	
	DWORD remainByte, sttFlag;
	int i;
	
	i = 0;
	if(WaitCommEvent(hComm, &sttFlag, NULL)){
		if(sttFlag == 1029){
			do{
				ReadFile(
					hComm,
					&(dataRx[i]),
					1,
					&remainByte,
					NULL
				);
				
				i++;
			} while(remainByte>0);
		}
	} else{
		printf("\nError al leer puerto %s...\n\n", portName);
	}
	
	*dataLen = i;
}

void writePort(char dataTx[], int dataLen){
	
	portStt = WriteFile(
		hComm,
		dataTx,
		dataLen,
		NULL,
		NULL
	);
	
	if(!portStt){
		printf("Error al escribir en el puerto...\n");
	}
}

void flushBuffer(){
	DWORD remainByte, sttFlag;
	char buff;

	if(WaitCommEvent(hComm, &sttFlag, NULL)){
		if(sttFlag == 1029){
			do{
				ReadFile(
					hComm,
					&buff,
					1,
					&remainByte,
					NULL
				);
			} while(remainByte>0);
		}
	} else{
		printf("\nError al leer puerto %s...\n\n", portName);
	}
}

void closePort(){
	CloseHandle(hComm);
	
	printf("\nPuerto %s cerrado...\n\n", portName);
}

