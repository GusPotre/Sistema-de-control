#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include <locale.h>
#include "com_port.h"
#include <ctype.h>
#include <math.h>

// Funci�n prototipo
void valor(int dim, char puerto[dim]);

int main() 
{
	setlocale(LC_CTYPE, "spanish");
	 
	int opcion; // Men� principal
	int desc; // Men� 1
	
	int caso; // Men� 2
	int modo; // men� 2.1
	int nivel; // Men� 2.2
	int L_min; // Men� 2.2.1
	int L_max; // Men� 2.2.2
	
	int valv; // Men� 3
	int Vint; // Men� 3.1
	int detener;
	int Vout; // Men� 3.2
	
	// Puerto
	char comPort[8];
	char prueba[5];
	
	// Lectura y escritura
	char dataRx[5];
	char dataRx1[8];
	char dataTx1[8];
	int nData, i;
	
	// Monitoreo (Men� 4)
	int opt = 0;
	
	char mode[5];
	
	char valv_int[4];
	char valv_out[4];
	char INvalv[8];
	char OUTvalv[8];
	
	char level_min[7];
	char level_max[7];
	char string_min[3];
	char string_max[3];
	int num_min;
	int num_max;
	
	float data_level;
	float alt;
	float level;
	float area;
	float volumen;
	
	char dataPa[13];
	char string_Pa[8];
	float presion;
	
	// Constantes 
	float PI = 3.1416;
	float altura = 3; // (m)
	float radio = 1.5; // (m) 
	float gravedad = 9.81;// (m/s^2)
	float densidad = 996; // (kg/m^3)

	sprintf(comPort, "NOpuerto");
	
	Menu: 
		system("cls");
		printf("|-----------------------------------------------------------------------------------------------|\n");
		printf("|------------------------------ Sistema de Nivel - Men� principal ------------------------------|\n");
		printf("|-----------------------------------------------------------------------------------------------|\n");
		valor(8, comPort);
		
		printf("\n1. Conexi�n");
		printf("\n2. Configurar controlador");
		printf("\n3. Configurar v�lvulas");
		printf("\n4. Monitoreo de variables");
		printf("\n5. Salir\n");
	
		printf("\nOpci�n: ");
		scanf("%i", &opcion);
		
		if((opcion < 5) && (opcion > 1) && strcmp(comPort, "NOpuerto") == 0)
		{
			system("cls");
			printf("|-----------------------------------------------------------------------------------------------|\n");
			printf("|----------------------------------- Mensaje de advertencia ------------------------------------|\n");
			printf("|-----------------------------------------------------------------------------------------------|\n");
			valor(8, comPort);
			printf("\nAcceso restringido ---> Controlador no conectado...\n\n");
			system("pause");
			goto Menu;
		}
		else{
			switch(opcion)
			{
				case 1:
					goto Conexion;
					break;
				case 2:
					goto Controlador;
					break;
				case 3:
					goto Valvulas;
					break;
				case 4:
					goto Monitoreo;
					break;
				case 5: 
					goto Salida;
					break;
				default:
					goto Menu;
			}
		}	
	
	// Men� 1
	Conexion:
		system("cls");
		printf("|-----------------------------------------------------------------------------------------------|\n");
		printf("|--------------------------------- Sistema de Nivel - Conexi�n ---------------------------------|\n");
		printf("|-----------------------------------------------------------------------------------------------|\n");
		valor(8, comPort);
		
		if(strcmp(comPort, "NOpuerto") == 0)
		{
			printf("\nIndique el puerto: ");
    		scanf("%s", comPort);
    		openPort(comPort);

			for(i=0; i<6; i++)
			{
				writePort("x00\r", 4);
    			Sleep(100);
    			readPort(prueba, &nData);
			}
    		
    		if(prueba[4] == 'M' || prueba[4] == 'A') // Comprobocar comunicaci�n 
    		{goto Menu;}
			else 
			{
				printf("\nError de conexi�n...\n");
				sprintf(comPort, "NOpuerto");
				system("pause"); goto Menu;
			}
			
    	}
    	else if(strncasecmp(comPort, "COM", 3) == 0)
		{
			printf("\n�Desconectar?");
			printf("\n1. Si");
			printf("\n2. No\n");
			printf("\nOpci�n: ");
			scanf("%i", &desc);
			
			switch(desc)
			{
				case 1:
					closePort(); 
					sprintf(comPort, "NOpuerto");
					sprintf(prueba, "00000"); 
					goto Menu;
					break;
				case 2:
					goto Menu;
					break;
				default:
					goto Conexion;
			}
		}
	
	// Men� 2	
	Controlador:
		system("cls");
		printf("|-----------------------------------------------------------------------------------------------|\n");
		printf("|-------------------------- Sistema de Nivel - Configurar controlador --------------------------|\n");
		printf("|-----------------------------------------------------------------------------------------------|\n");
		valor(8, comPort);
		
		printf("\n1. Modo de operaci�n");
		printf("\n2. L�mites de nivel");
		printf("\n3. Regresar\n");
		
		printf("\nOpci�n: ");
		scanf("%i", &caso);
		
		switch(caso)
		{
			case 1:
				goto Modo_operacion;
				break;
			case 2: 
				goto Limite_nivel;
				break;
			case 3:
				goto Menu;
				break;
		}
			
	// Men� 2.1
	Modo_operacion:
		system("cls");
		printf("|-----------------------------------------------------------------------------------------------|\n");
		printf("|----------------- Sistema de Nivel - Configurar controlador - Modo de operaci�n ---------------|\n");
		printf("|-----------------------------------------------------------------------------------------------|\n");
		valor(8, comPort);
		
		printf("\n1. Configurar modo autom�tico");
		printf("\n2. Configurar modo manual");
		printf("\n3. Regresar\n");
		
		printf("Opci�n: ");
		scanf("%i", &modo);
		
		switch(modo)
		{
			case 1:
				writePort("x10:A\r", 6);
				Sleep(10);
				readPort(dataRx1, &nData);
				goto Modo_operacion; 
				break;
			case 2:
				writePort("x10:M\r", 6);
				Sleep(10);
				readPort(dataRx1, &nData);
				goto Modo_operacion; 
				break;
			case 3:
				goto Controlador;
				break;
		}

	// Men� 2.2 	
	Limite_nivel:
		system("cls");
		printf("|-----------------------------------------------------------------------------------------------|\n");
		printf("|----------------- Sistema de Nivel - Configurar controlador - L�mites de nivel ----------------|\n");
		printf("|-----------------------------------------------------------------------------------------------|\n");
		valor(8, comPort);
		
		printf("\n1. Configurar l�mite m�nimo");
		printf("\n2. Configurar l�mite m�ximo");
		printf("\n3. Regresar\n");
		
		printf("Opcion: ");
		scanf("%i", &nivel);
		
		switch(nivel)
		{
			case 1:
				goto Lmin;
				break;
			case 2: 
				goto Lmax;
				break;
			case 3:
				goto Controlador;
				break;
		}
	
	// Men� 2.2.1 
	Lmin:
		system("cls");
		printf("|-----------------------------------------------------------------------------------------------|\n");
		printf("|-------------- Sistema de Nivel - Configurar controlador - L�mites de nivel: Lmin -------------|\n");
		printf("|-----------------------------------------------------------------------------------------------|\n");
		valor(8, comPort);
		
		printf("\nDigite un nivel m�nimo entre 0 y 100: ");
		scanf("%i", &L_min);
		
		if(L_min >= 0 && L_min <= 100)
		{
			sprintf(dataTx1, "x13:%i\r", L_min);
			writePort(dataTx1, 8);
			Sleep(10);
			readPort(dataRx1, &nData);
		}
		else
		{
			system("cls");
			printf("|-----------------------------------------------------------------------------------------------|\n");
			printf("|----------------------------------- Mensaje de advertencia ------------------------------------|\n");
			printf("|-----------------------------------------------------------------------------------------------|\n");
			valor(8, comPort);
			printf("\nDato no valido ---> Fuera del rango...\n\n");
			system("pause");
		}
			goto Limite_nivel;
		
	// Men� 2.2.2
	Lmax:
		system("cls");
		printf("|-----------------------------------------------------------------------------------------------|\n");
		printf("|-------------- Sistema de Nivel - Configurar controlador - L�mites de nivel: Lmax -------------|\n");
		printf("|-----------------------------------------------------------------------------------------------|\n");
		valor(8, comPort);
		
		printf("\nDigite un nivel m�ximo entre 0 y 100: ");
		scanf("%i", &L_max);
		
		if(L_max >= 0 && L_max <= 100)
		{
			sprintf(dataTx1, "x14:%i\r", L_max);
			writePort(dataTx1, 8);
			Sleep(10);
			readPort(dataRx1, &nData);
		}
		else
		{
			system("cls");
			printf("|-----------------------------------------------------------------------------------------------|\n");
			printf("|----------------------------------- Mensaje de advertencia ------------------------------------|\n");
			printf("|-----------------------------------------------------------------------------------------------|\n");
			valor(8, comPort);
			printf("\nDato no valido ---> Fuera del rango...\n\n");
			system("pause");
		}	
		goto Limite_nivel;
		
	// Men� 3
	Valvulas:
		system("cls");
		printf("|-----------------------------------------------------------------------------------------------|\n");
		printf("|---------------------------- Sistema de Nivel - Configurar v�lvulas ---------------------------|\n");
		printf("|-----------------------------------------------------------------------------------------------|\n");
		valor(8, comPort);
		
		printf("\n1. V�lvula de entrada");
		printf("\n2. V�lvula de salida");
		printf("\n3. Regresar\n");
		
		printf("\nOpci�n: ");
		scanf("%i", &valv);
		
		switch(valv)
		{
			case 1:
				goto Valv_int;
				break;
			case 2:
				goto Valv_out;
				break;
			case 3:
				goto Menu;
				break;
		}
	
	// Men� 3.1
	Valv_int:
		system("cls");
		printf("|-----------------------------------------------------------------------------------------------|\n");
		printf("|----------------- Sistema de Nivel - Configurar v�lvulas - V�lvula de entrada -----------------|\n");
		printf("|-----------------------------------------------------------------------------------------------|\n");
		valor(8, comPort);
		
		printf("\n1. Abrir v�lvula");
		printf("\n2. Cerrar v�lvula");
		printf("\n3. Regresar\n");
		
		printf("\n Opci�n: ");
		scanf("%i", &Vint);
		
		switch(Vint)
		{
			case 1:
				goto Abrir_int;
				break;
			case 2:
				goto Cerrar_int;
				break;
			case 3:
				goto Valvulas;
				break;
		}
		
	Abrir_int:
		writePort("x03\r", 4); // ZSO
		Sleep(10);
		readPort(dataRx, &nData);
		valv_int[2] = dataRx[4];
		
		if(valv_int[2] == '0')
		{
			system("cls");
			printf("|-----------------------------------------------------------------------------------------------|\n");
			printf("|----------------- Sistema de Nivel - Configurar v�lvulas - V�lvula de entrada -----------------|\n");
			printf("|-----------------------------------------------------------------------------------------------|\n");
			valor(8, comPort);
		 
			writePort("x11:O\r", 6); // Abrir
			Sleep(10);
			readPort(dataRx, &nData);
			printf("\n\tAbriendo v�lvula");
			detener = 1;
			
			while(detener != 0)
			{
				writePort("x01\r", 4); // SVO
				Sleep(250);
				readPort(dataRx, &nData);
				valv_int[0] = dataRx[4];
				printf(".");
				if(valv_int[0] == '0')
				{detener = 0;}	
			}
		}
		else if(valv_int[2] == '1')
		{
			system("cls");
			printf("|-----------------------------------------------------------------------------------------------|\n");
			printf("|----------------- Sistema de Nivel - Configurar v�lvulas - V�lvula de entrada -----------------|\n");
			printf("|-----------------------------------------------------------------------------------------------|\n");
			valor(8, comPort);
			printf("\n�La v�lvula ya se encuentra ABIERTA!\n\n");
			system("pause");
		}
		goto Valv_int;
		
	Cerrar_int:
		writePort("x04\r", 4); // ZSC
		Sleep(10);
		readPort(dataRx, &nData);
		valv_int[3] = dataRx[4];
		
		if(valv_int[3] == '0')
		{
			system("cls");
			printf("|-----------------------------------------------------------------------------------------------|\n");
			printf("|----------------- Sistema de Nivel - Configurar v�lvulas - V�lvula de entrada -----------------|\n");
			printf("|-----------------------------------------------------------------------------------------------|\n");
			valor(8, comPort);
		 
			writePort("x11:C\r", 6); // Cerrar
			Sleep(10);
			readPort(dataRx, &nData);
			printf("\n\tCerrando v�lvula");
			detener = 1;
			
			while(detener != 0)
			{
				writePort("x02\r", 4); // SVC
				Sleep(250);
				readPort(dataRx, &nData);
				valv_int[1] = dataRx[4];
				printf(".");
				if(valv_int[1] == '0')
				{detener = 0;}	
			}
		}
		else if(valv_int[3] == '1')
		{
			system("cls");
			printf("|-----------------------------------------------------------------------------------------------|\n");
			printf("|----------------- Sistema de Nivel - Configurar v�lvulas - V�lvula de entrada -----------------|\n");
			printf("|-----------------------------------------------------------------------------------------------|\n");
			valor(8, comPort);
			printf("\n�La v�lvula ya se encuentra CERRRADA!\n\n");
			system("pause");
		}
		goto Valv_int;
		
	// Men� 3.2
	Valv_out:
		system("cls");
		printf("|-----------------------------------------------------------------------------------------------|\n");
		printf("|------------------ Sistema de Nivel - Configurar v�lvulas - V�lvula de salida -----------------|\n");
		printf("|-----------------------------------------------------------------------------------------------|\n");
		valor(8, comPort);
		
		printf("\n1. Abrir v�lvula");
		printf("\n2. Cerrar v�lvula");
		printf("\n3. Regresar\n");
		
		printf("\nOpci�n: ");
		scanf("%i", &Vout);
		
		switch(Vout)
		{
			case 1:
				goto Abrir_out;
				break;
			case 2:
				goto Cerrar_out;
				break;
			case 3:
				goto Valvulas;
				break;
		}
	
	Abrir_out:
		writePort("x07\r", 4); // ZSO
		Sleep(10);
		readPort(dataRx, &nData);
		valv_out[2] = dataRx[4];
		
		if(valv_out[2] == '0')
		{
			system("cls");
			printf("|-----------------------------------------------------------------------------------------------|\n");
			printf("|------------------ Sistema de Nivel - Configurar v�lvulas - V�lvula de salida -----------------|\n");
			printf("|-----------------------------------------------------------------------------------------------|\n");
			valor(8, comPort);
		 
			writePort("x12:O\r", 6); // Abrir
			Sleep(10);
			readPort(dataRx, &nData);
			printf("\n\tAbriendo v�lvula");
			detener = 1;
			
			while(detener != 0)
			{
				writePort("x05\r", 4); // SVO
				Sleep(250);
				readPort(dataRx, &nData);
				valv_out[0] = dataRx[4];
				printf(".");
				if(valv_out[0] == '0')
				{detener = 0;}	
			}
		}
		else if(valv_out[2] == '1')
		{
			system("cls");
			printf("|-----------------------------------------------------------------------------------------------|\n");
			printf("|------------------ Sistema de Nivel - Configurar v�lvulas - V�lvula de salida -----------------|\n");
			printf("|-----------------------------------------------------------------------------------------------|\n");
			valor(8, comPort);
			printf("\n�La v�lvula ya se encuentra ABIERTA!\n\n");
			system("pause");
		}
		goto Valv_out;
	
	Cerrar_out:
		writePort("x08\r", 4); // ZSC
		Sleep(10);
		readPort(dataRx, &nData);
		valv_out[3] = dataRx[4];
		
		if(valv_out[3] == '0')
		{
			system("cls");
			printf("|-----------------------------------------------------------------------------------------------|\n");
			printf("|------------------ Sistema de Nivel - Configurar v�lvulas - V�lvula de salida -----------------|\n");
			printf("|-----------------------------------------------------------------------------------------------|\n");
			valor(8, comPort);
		 
			writePort("x12:C\r", 6); // Cerrar
			Sleep(10);
			readPort(dataRx, &nData);
			printf("\n\tCerrando v�lvula");
			detener = 1;
			
			while(detener != 0)
			{
				writePort("x06\r", 4); // SVC
				Sleep(250);
				readPort(dataRx, &nData);
				valv_out[1] = dataRx[4];
				printf(".");
				if(valv_out[1] == '0')
				{detener = 0;}	
			}
		}
		else if(valv_out[3] == '1')
		{
			system("cls");
			printf("|-----------------------------------------------------------------------------------------------|\n");
			printf("|------------------ Sistema de Nivel - Configurar v�lvulas - V�lvula de salida -----------------|\n");
			printf("|-----------------------------------------------------------------------------------------------|\n");
			valor(8, comPort);
			printf("\n�La v�lvula ya se encuentra CERRRADA!\n\n");
			system("pause");
		}
		goto Valv_out;
		
	// Men� 4
	Monitoreo:
		system("cls");
		opt = 0;
		while(opt == 0)
		{
				
			writePort("x00\r", 4); // Lectura de modo
			readPort(mode, &nData);
			if(mode[4] == 'M') {}
			else if(mode[4] == 'A') {}
			else {sprintf(mode, "---");}
			
			// V�lvula de entrada
			writePort("x01\r", 4); // SVO
			readPort(dataRx, &nData);
			valv_int[0] = dataRx[4];
			
			writePort("x02\r", 4); // SVC
			readPort(dataRx, &nData);
			valv_int[1] = dataRx[4];
			
			writePort("x03\r", 4); // ZSO
			readPort(dataRx, &nData);
			valv_int[2] = dataRx[4];
			
			writePort("x04\r", 4); // ZSC
			readPort(dataRx, &nData);
			valv_int[3] = dataRx[4];
			
			if(valv_int[0] == '1' && valv_int[1] == '0')
			{sprintf(INvalv, "Abriendo");}
			else if(valv_int[1] == '1' && valv_int[0] == '0')
			{sprintf(INvalv, "Cerrando");}
			else if(valv_int[2] == '1' && valv_int[3] == '0')
			{sprintf(INvalv, "Abierto");}
			else if(valv_int[3] == '1' && valv_int[2] == '0')
			{sprintf(INvalv, "Cerrado");}
			else
			{sprintf(INvalv, "---");}
			
			// V�lvula de salida
			writePort("x05\r", 4); // SVO
			readPort(dataRx, &nData);
			valv_out[0] = dataRx[4];
			
			writePort("x06\r", 4); // SVC
			readPort(dataRx, &nData);
			valv_out[1] = dataRx[4];
			
			writePort("x07\r", 4); // ZSO
			readPort(dataRx, &nData);
			valv_out[2] = dataRx[4];
			
			writePort("x08\r", 4); // ZSC
			readPort(dataRx, &nData);
			valv_out[3] = dataRx[4];
			
			if(valv_out[0] == '1' && valv_out[1] == '0')
			{sprintf(OUTvalv, "Abriendo");}
			else if(valv_out[1] == '1' && valv_out[0] == '0')
			{sprintf(OUTvalv, "Cerrando");}
			else if(valv_out[2] == '1' && valv_out[3] == '0')
			{sprintf(OUTvalv, "Abierto");}
			else if(valv_out[3] == '1' && valv_out[2] == '0')
			{sprintf(OUTvalv, "Cerrado");}
			else
			{sprintf(OUTvalv, "---");}
			
			writePort("x0A\r", 4);	// Lectura de nivel min
			readPort(level_min, &nData);
			string_min[0] = level_min[4];
			string_min[1] = level_min[5];
			string_min[2] = level_min[6];
			num_min = atoi(string_min);
			
			writePort("x0B\r", 4);	// Lectura de nivel max
			readPort(level_max, &nData);
			string_max[0] = level_max[4];
			string_max[1] = level_max[5];
			string_max[2] = level_max[6];
			num_max = atoi(string_max);
		
			writePort("x09\r", 4); // Lectura de presi�n
			readPort(dataPa, &nData);
			string_Pa[0] = dataPa[4];
			string_Pa[1] = dataPa[5];
			string_Pa[2] = dataPa[6];
			string_Pa[3] = dataPa[7];
			string_Pa[4] = dataPa[8];
			string_Pa[5] = dataPa[9];
			string_Pa[6] = dataPa[10];
			string_Pa[7] = dataPa[11];
			presion = atof(string_Pa);
			
			// Nivel
			data_level = presion / (densidad * gravedad);
			alt = data_level;
			level = data_level * 100 / altura;
			
			// Volumen 
			area = PI * pow(radio, 2); 
			volumen = 1000 * (area * alt);
			
			printf("\n Modo: %c", mode[4]);
			printf(" | Val_in: %s", INvalv);
			printf(" | Val_out: %s", OUTvalv);
			printf(" | Lmin: %i%%", num_min);
			printf(" | Lmax: %i%%", num_max);
			printf(" | L: %.2f%%", level);
			printf(" | Vol: %.2f lts", volumen);
			printf(" | Presi�n: %.2f Pa", presion);
			
			opt = kbhit();
			Sleep(500);
		}
		printf("\n\n");
		system("pause");
		goto Menu;
	
	// Salir del programa
	Salida:
		if(strcmp(comPort, "NOpuerto") == 0)
		{
			system("cls");
			printf("|-----------------------------------------------------------------------------------------------|\n");
			printf("|--------------------------- Sistema de Nivel - Ejecuci�n finalizada ---------------------------|\n");
			printf("|-----------------------------------------------------------------------------------------------|\n");
			valor(8, comPort);
			closePort();
		}
		if(strncasecmp(comPort, "COM", 3) == 0)
		{
			system("cls");
			printf("|-----------------------------------------------------------------------------------------------|\n");
			printf("|------------------------------------ Mensaje de advertencia -----------------------------------|\n");
			printf("|-----------------------------------------------------------------------------------------------|\n");
			valor(8, comPort);
			printf("\nDenegado ---> El controlador sigue conectado ---> Puerto: %s\n\n", comPort);
			system("pause");
			goto Menu;
		}
	return 0;
}

void valor(int dim, char puerto[dim])
{
	char modo[5];
	char Lmin[7];
	char Lmax[7];
	int nData, i;
	
	char Cmin[3];
	char Cmax[3];
	int num_min = 0;
	int num_max = 0;
	
	if(strcmp(puerto, "NOpuerto") == 0)
	{
		printf("\nConectado: ---");
		printf("\t| Modo: ---");
		printf("\t| L_min: ---");
		printf("\t| L_max: ---");
		printf("\n---\n");
	}
	else if(strncasecmp(puerto, "COM", 3) == 0)
	{
		for(i=0; i<8; i++)
		{puerto[i] = toupper(puerto[i]);}
		
		for(i=0; i<2; i++) // Lectura de modo
			{
				writePort("x00\r", 4);	
				Sleep(10);
				readPort(modo, &nData);
			}	
	
		writePort("x0A\r", 4);	// Lectura de nivel min
		Sleep(10);
		readPort(Lmin, &nData);
		Cmin[0] = Lmin[4];
		Cmin[1] = Lmin[5];
		Cmin[2] = Lmin[6];
		num_min = atoi(Cmin);
		
		writePort("x0B\r", 4);	// Lectura de nivel max
		Sleep(10);
		readPort(Lmax, &nData);
		Cmax[0] = Lmax[4];
		Cmax[1] = Lmax[5];
		Cmax[2] = Lmax[6];
		num_max = atoi(Cmax);
		
		// Valores
		printf("\nConectado: %s", puerto);
		printf(" | Modo: %c", modo[4]);
		printf(" | Lmin: %i%%", num_min);
		printf(" | Lmax: %i%%", num_max);
		printf("\n---\n");
	}
}
/*
	Aldama Tello Denise 4J
	Sanchez Potrero Gustavo 
	Vazquez Vazquez Marco Antonio
*/
