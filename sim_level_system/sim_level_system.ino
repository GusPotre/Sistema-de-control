/*  Universidad Tecnológica de Puebla
 *  División de Mecatrónica
 *  
 *  Por: Dr. Carlos J. Morales
 *    Programa básico de simulación de un control y monitoreo
 *	  de un sistema de nivel.
 *    (nov 2022)
 *    
 *  Pines:
 *		13 : PBO_in		[Botón (entrada, activo en alto)]
 *		12 : PBC_in		[Botón (entrada, activo en alto)]
 *		11 : ZSO_in		[LED (salida)]
 *		10 : ZSC_in		[LED (salida)]
 *		 9 : SVO_in		[LED (salida)]
 *		 8 : SVC_in		[LED (salida)]
 *		 7 : PBO_out	[Botón (entrada, activo en alto)] 
 *		 6 : PBC_out	[Botón (entrada, activo en alto)]
 *		 5 : ZSO_out	[LED (salida)]
 *		 4 : ZSC_out	[LED (salida)]
 *		 3 : SVO_out	[LED (salida)]
 *		 2 : SVC_out 	[LED (salida)]
 */

#include <TimerOne.h>

#define Qi_max 0.15
#define dt 0.1
#define r 1.5
#define h_max 3
#define gamma 996

#define Ao 0.01
#define Ab 3.1416 * r * r
#define Abi 0.14147
#define Ch 9.81 * gamma
#define V_max Ab*h_max

#define PBO_in 13
#define PBC_in 12
#define ZSO_in 11
#define ZSC_in 10
#define SVO_in 9
#define SVC_in 8
#define PBO_out 7
#define PBC_out 6
#define ZSO_out 5
#define ZSC_out 4
#define SVO_out 3
#define SVC_out 2

int Lmin, Lmax;

float P, h, L, Qi, Qo, Qvi, Qvo, Vtot, Cvi, Cvo;

char mode;
byte Valves;


void setup() {
  // put your setup code here, to run once:
  pinMode(PBO_in, INPUT);
  pinMode(PBC_in, INPUT);
  pinMode(ZSO_in, OUTPUT);
  pinMode(ZSC_in, OUTPUT);
  pinMode(SVO_in, OUTPUT);
  pinMode(SVC_in, OUTPUT);
  pinMode(PBO_out, INPUT);
  pinMode(PBC_out, INPUT);
  pinMode(ZSO_out, OUTPUT);
  pinMode(ZSC_out, OUTPUT);
  pinMode(SVO_out, OUTPUT);
  pinMode(SVC_out, OUTPUT);
  
  Serial.begin(9600);
  Serial.setTimeout(5000);

  P = 0.0;
  Qi = 0.0;
  Qo = 0.0;
  Cvi = 0.0;
  Cvo = 0.0;
  Vtot = 0.0;
  mode = 'M';
  Lmax = 80;
  Lmin = 30;
  Valves  = 0b10001000;

  Timer1.initialize(100000);              // Activación cada 100ms
  Timer1.attachInterrupt(ISR_computing);
}

void loop() {
  // put your main code here, to run repeatedly:
  int Ltm, tmp;

  if(Serial.available()>0){
    String dataRx = Serial.readStringUntil('\r');
    int flag = 0;
    dataRx.trim();
    if((dataRx.charAt(0)=='x' || dataRx.charAt(0)=='X')  && dataRx.charAt(1)=='0'){
      switch (dataRx.charAt(2)){
        /* --- Modo lectura ------------------------------ */
        case 48:  // Modo: Automático-Manual
          Serial.print("x00:");
          Serial.println(mode);
          flag = 1;
          break;
        case 49:  // Estado de SVO de la válvula de entrada.
          if(bitRead(Valves, 0)==1){
            Serial.println("x01:1");
          }else{
            Serial.println("x01:0");
          }
          flag = 1;
          break;
        case 50:  // Estado de SVC de la válvula de entrada.
          if(bitRead(Valves, 1)==1){
            Serial.println("x02:1");
          }else{
            Serial.println("x02:0");
          }
          flag = 1;
          break;
        case 51:  // Estado de ZSO de la válvula de entrada
          if(bitRead(Valves, 2)==1){
            Serial.println("x03:1");
          }else{
            Serial.println("x03:0");
          }
          flag = 1;
          break;
        case 52:  // Estado de ZSC de la válvula de entrada
        if(bitRead(Valves, 3)==1){
            Serial.println("x04:1");
          }else{
            Serial.println("x04:0");
          }
          flag = 1;
          break;
        case 53:  // Estado de SVO de la válvula de salida
          if(bitRead(Valves, 4)==1){
            Serial.println("x05:1");
          }else{
            Serial.println("x05:0");
          }
          flag = 1;
          break;
        case 54:  // Estado de SVC de la válvula de salida
          if(bitRead(Valves, 5)==1){
            Serial.println("x06:1");
          }else{
            Serial.println("x06:0");
          }
          flag = 1;
          break;
        case 55:  // Estado de ZSO de la válvula de salida
          if(bitRead(Valves, 6)==1){
            Serial.println("x07:1");
          }else{
            Serial.println("x07:0");
          }
          flag = 1;
          break;
        case 56:  // Estado de ZSC de la válvula de salida
          if(bitRead(Valves, 7)==1){
            Serial.println("x08:1");
          }else{
            Serial.println("x08:0");
          }
          flag = 1;
          break;
        case 57:  // Lectura de presión
          Serial.print("x09:");
          Serial.println(P);
          flag = 1;
          break;
        case 65:  // Lectura de Lmin
          Serial.print("x0A:");
          Serial.println(Lmin);
          flag = 1;
          break;
        case 66:  // Lectura de Lmax
          Serial.print("x0B:");
          Serial.println(Lmax);
          flag = 1;
          break;
      }
    } else if((dataRx.charAt(0)=='x' || dataRx.charAt(0)=='X') && dataRx.charAt(1)=='1'){
      /* --- Modo escritura ---------------------------- */
      switch(dataRx.charAt(2)){
        case 48:  // Configurar modo: Automatico/Manual
          if(dataRx.charAt(4)=='M'){
            mode = 'M';
            flag = 1;
          } else if(dataRx.charAt(4)=='A'){
            mode = 'A';
            flag = 1;
          }
          break;
        case 49:  // Operar válvula de entrada
          if(dataRx.charAt(4)=='O'){
            bitWrite(Valves, 0, 1);
            bitWrite(Valves, 1, 0);
            flag = 1;
          } else if(dataRx.charAt(4)=='C'){
            bitWrite(Valves, 0, 0);
            bitWrite(Valves, 1, 1);
            flag = 1;
          }
          break;
        case 50:  // Operar válvula de salida
          if(dataRx.charAt(4)=='O'){
            bitWrite(Valves, 4, 1);
            bitWrite(Valves, 5, 0);
            flag = 1;
          } else if(dataRx.charAt(4)=='C'){
            bitWrite(Valves, 4, 0);
            bitWrite(Valves, 5, 1);
            flag = 1;
          }
          break;
        case 51:  // Escribir Lmin
          Ltm = 0;
          tmp  = 1;
          for(int i=dataRx.length()-1; i>3; i--){
            Ltm += tmp * (dataRx.charAt(i) - 48);
            tmp  = tmp * 10; 
          }

          Lmin = Ltm;
          flag = 1;
          break;
        case 52:  // Escribir Lmax
          Ltm = 0;
          tmp = 1;
          for(int i=dataRx.length()-1; i>3; i--){
            Ltm += tmp * (dataRx.charAt(i) - 48);
            tmp  = tmp * 10; 
          }

          Lmax = Ltm;
          flag = 1;
          break;
      /* ----------------------------------------------- */
      }
    }

    if(flag==0){
      Serial.print('x');
      Serial.print(dataRx[1]);
      Serial.print(dataRx[2]);
      Serial.println(":_NK_");
    } else if(flag==1 && dataRx.charAt(1)=='1'){
      Serial.print('x');
      Serial.print(dataRx[1]);
      Serial.print(dataRx[2]);
      Serial.println(":RDY");
    }
  }
}
  
void ISR_computing(){
  /* --- Control de válvulas --------------------------- */
  if(bitRead(Valves, 0)==1){         // Abrir válvula de entrada
    if(Cvi<1.0){
      Cvi = Cvi + 0.01;
      bitWrite(Valves, 1, 0);
      bitWrite(Valves, 2, 0);
      bitWrite(Valves, 3, 0);
    } else{
      Cvi = 1.0;
      bitWrite(Valves, 0, 0);
      bitWrite(Valves, 2, 1);
      bitWrite(Valves, 3, 0);
    }
  } else if(bitRead(Valves, 1)==1){  // Cerrar válvula de entrada
    if(Cvi>0.0){
      Cvi = Cvi - 0.01;
      bitWrite(Valves, 0, 0);
      bitWrite(Valves, 2, 0);
      bitWrite(Valves, 3, 0);
    } else{
      Cvi = 0.0;
      bitWrite(Valves, 1, 0);
      bitWrite(Valves, 2, 0);
      bitWrite(Valves, 3, 1);
    }
  }

  if(bitRead(Valves, 4)==1){         // Abrir válvula de salida
    if(Cvo<1.0){
      Cvo = Cvo + 0.01;
      bitWrite(Valves, 5, 0);
      bitWrite(Valves, 6, 0);
      bitWrite(Valves, 7, 0);
    } else{
      Cvo = 1.0;
      bitWrite(Valves, 4, 0);
      bitWrite(Valves, 6, 1);
      bitWrite(Valves, 7, 0);
    }
  } else if(bitRead(Valves, 5)==1){  // Cerrar válvula de salida
    if(Cvo>0.0){
      Cvo = Cvo - 0.01;
      bitWrite(Valves, 4, 0);
      bitWrite(Valves, 6, 0);
      bitWrite(Valves, 7, 0);
    } else{
      Cvo = 0.0;
      bitWrite(Valves, 5, 0);
      bitWrite(Valves, 6, 0);
      bitWrite(Valves, 7, 1);
    }
  }

  /* --- Simulación de llenado/vaciado ----------------- */
  Qi = Cvi * Qi_max;
  Qo = Cvo * Ao * sqrt(h);

  Vtot += dt * (Qi - Qo);

  if(Vtot>V_max){
    Vtot = V_max;
  } else if(Vtot<0.0){
    Vtot = 0.0;
  }

  h = Vtot * Abi;
  P = h * Ch;
  L = 100 * h / h_max;

  if(mode=='A'){
    if(int(L)>=Lmax && bitRead(Valves, 2)==1){
      bitWrite(Valves, 1, 1);
    } else if(int(L)<=Lmin && bitRead(Valves, 3)==1){
      bitWrite(Valves, 0, 1);
    }
  }

  ctrlPer();
  /* --------------------------------------------------- */
}


void ctrlPer(){  
  if(mode=='M'){
    if(digitalRead(PBO_in)==1){
      bitWrite(Valves, 0, 1);
    }

    if(digitalRead(PBC_in)==1){
      bitWrite(Valves, 1, 1);
    }
  }

  if(digitalRead(PBO_out)==1){
    bitWrite(Valves, 4, 1);
  }

  if(digitalRead(PBC_out)==1){
    bitWrite(Valves, 5, 1);
  }

  if(bitRead(Valves, 0)==1){
    digitalWrite(SVO_in, HIGH);
  } else{
    digitalWrite(SVO_in, LOW);
  }

  if(bitRead(Valves, 1)==1){
    digitalWrite(SVC_in, HIGH);
  } else{
    digitalWrite(SVC_in, LOW);
  }

  if(bitRead(Valves, 2)==1){
    digitalWrite(ZSO_in, HIGH);
  } else{
    digitalWrite(ZSC_in, LOW);
  }

  if(bitRead(Valves, 3)==1){
    digitalWrite(ZSC_in, HIGH);
  } else{
    digitalWrite(ZSC_in, LOW);
  }

  if(bitRead(Valves, 4)==1){
    digitalWrite(SVO_out, HIGH);
  } else{
    digitalWrite(SVO_out, LOW);
  }

  if(bitRead(Valves, 5)==1){
    digitalWrite(SVC_out, HIGH);
  } else{
    digitalWrite(SVC_out, LOW);
  }

  if(bitRead(Valves, 6)==1){
    digitalWrite(ZSO_out, HIGH);
  } else{
    digitalWrite(ZSO_out, LOW);
  }

  if(bitRead(Valves, 7)==1){
    digitalWrite(ZSC_out, HIGH);
  } else{
    digitalWrite(ZSC_out, LOW);
  }
}
