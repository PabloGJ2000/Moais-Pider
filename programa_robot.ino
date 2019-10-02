/**
 *  Nombre del Proyecto: Moais'Pider 
 *  Idea orginal: Pablo García Jaén
 *  Copyright 2016 - MoaisEnergy©
 *  www.moaisenergy.wordpress.com
 *  Última modificicación: 19/06/2016
 *  Modificado por: Pablo García Jaén
 *  Publicado bajo licencia CC-BY-SA 4.0
 *  Creative Commons: Reconocimiento - Compartir Igual 4.0 Internacional
 *  http://creativecommons.org/licenses/by-sa/4.0/deed.es_ES
 */

/* Nomenclatura de los servos
 *  D=Derecha
 *  I=Izquierda
 *  F=Frontal
 *  T=Trasera
 *  V=Vertical
 *  H=Horizontal
 */
#include <Servo.h>
#include <SPI.h>
#include "RF24.h"

const int NRF24_CE_PIN = 9;  // Pin CE del NRF24
const int NRF24_CS_PIN = 10; // Pin CS del NRF24

RF24 radio(NRF24_CE_PIN, NRF24_CS_PIN); // Crea la radio
/**
 * TUBERÍA DE COMUNICACIÓN
 * 
 * Una tubería de comunicación es un enlace entre dos radios.
 * 
 * Cada tubería se identifica con una combinación de 6 bytes
 * que debe ser diferente entre distintas tuberías. Esta
 * combinación se escribe en hexadecimal por claridad (a todo
 * buen programador le es más claro el hexadecimal cuando
 * hablamos de bytes, así que ya te puedes acostumbrar).
 * 
 * Para comunicar dos radios, una envía datos por la tubería
 * mientras otra escucha. Ambas en la misma tubería.
 * 
 * Donde dice "tuberia1" puedes ponerle el nombre que quieras.
 */
byte tuberia1[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//byte tuberia2[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x01};

char lecturaRadio = 0; // Para guardar lo que recibimos


Servo servoDFV;
Servo servoDFH;
Servo servoDM;
Servo servoDTV;
Servo servoDTH;
Servo servoIFV;
Servo servoIFH;
Servo servoIM;
Servo servoITV;
Servo servoITH;

// Variables angulo servos
const int SERVO_DFV_ABAJO=120;  
const int SERVO_DFV_ARRIBA=90;

const int SERVO_DFH_ATRAS=88;
const int SERVO_DFH_MEDIO=122;

const int SERVO_DM_ABAJO=120;
const int SERVO_DM_ARRIBA=90;

const int SERVO_DTV_ABAJO=80;
const int SERVO_DTV_ARRIBA=120;

const int SERVO_DTH_ATRAS=157;
const int SERVO_DTH_MEDIO=116;

const int SERVO_IFV_ABAJO=60;
const int SERVO_IFV_ARRIBA=92;

const int SERVO_IFH_ATRAS=105;
const int SERVO_IFH_MEDIO=69;

const int SERVO_IM_ABAJO=63;
const int SERVO_IM_ARRIBA=95;

const int SERVO_ITV_ABAJO=85;
const int SERVO_ITV_ARRIBA=57;

const int SERVO_ITH_ATRAS=27;
const int SERVO_ITH_MEDIO=61;

// Variables tiempo
int tiempocorto=125;
int tiempo=250;
char manoizquierda=0;
char manoderecha=0;
// Variable controlar robot
int funcionrobot=0;

// Funciones del robot
void izqdescansa() {
  servoIFV.write(SERVO_IFV_ARRIBA);
  servoIFH.write(SERVO_IFH_MEDIO);
  servoIM.write(SERVO_IM_ARRIBA);
  servoITV.write(SERVO_ITV_ARRIBA);
  servoITH.write(SERVO_ITH_MEDIO);
}

void derdescansa() {
  servoDFV.write(SERVO_DFV_ARRIBA);
  servoDFH.write(SERVO_DFH_MEDIO);
  servoDM.write(SERVO_DM_ARRIBA);
  servoDTV.write(SERVO_DTV_ARRIBA);
  servoDTH.write(SERVO_DTH_MEDIO);
}

void derabajo () {
  servoDFV.write(SERVO_DFV_ABAJO);
  servoDFH.write(SERVO_DFH_MEDIO);
  servoDM.write(SERVO_DM_ABAJO);
  servoDTV.write(SERVO_DTV_ABAJO);
  servoDTH.write(SERVO_DTH_MEDIO);
}

void izqabajo () {
  servoIFV.write(SERVO_IFV_ABAJO);
  servoIFH.write(SERVO_IFH_MEDIO);
  servoIM.write(SERVO_IM_ABAJO);
  servoITV.write(SERVO_ITV_ABAJO);
  servoITH.write(SERVO_ITH_MEDIO);
}

void izqavanza () {
  servoITV.write(SERVO_ITV_ARRIBA);
  servoIM.write(SERVO_IM_ARRIBA);
  servoDM.write(SERVO_DM_ABAJO);
  servoDFV.write(SERVO_DFV_ARRIBA);
  servoDTV.write(SERVO_DTV_ARRIBA);
  servoIFV.write(SERVO_IFV_ABAJO);
  servoITH.write(SERVO_ITH_ATRAS);
  delay(tiempocorto);
  servoITV.write(SERVO_ITV_ABAJO);
  delay(tiempocorto);
  servoIFH.write(SERVO_IFH_ATRAS);
  servoITH.write(SERVO_ITH_MEDIO);
  delay(tiempocorto);
  servoIM.write(SERVO_IM_ABAJO);
  delay(tiempocorto);
  servoDFV.write(SERVO_DFV_ABAJO);
  servoDTV.write(SERVO_DTV_ABAJO);
  servoIFV.write(SERVO_IFV_ARRIBA);
  servoITV.write(SERVO_ITV_ARRIBA);
  delay(tiempocorto);
  servoIFH.write(SERVO_IFH_MEDIO);
  servoITH.write(SERVO_ITH_MEDIO);
  delay(tiempocorto);
  servoDM.write(SERVO_DM_ARRIBA);
  servoIFV.write(SERVO_IFV_ABAJO);
  servoITV.write(SERVO_ITV_ABAJO);
  return;
}

void deravanza () {
  servoDTV.write(SERVO_DTV_ARRIBA);
  servoIM.write(SERVO_IM_ABAJO);
  servoIFV.write(SERVO_IFV_ARRIBA);
  servoITV.write(SERVO_ITV_ARRIBA);
  servoDFV.write(SERVO_DFV_ABAJO);
  servoDTH.write(SERVO_DTH_ATRAS);
  delay(tiempocorto);
  servoDTV.write(SERVO_DTV_ABAJO);
  delay(tiempocorto);
  servoDFH.write(SERVO_DFH_ATRAS);
  servoDTH.write(SERVO_DTH_MEDIO);
  delay(tiempocorto);
  servoDM.write(SERVO_DM_ABAJO);
  delay(tiempocorto);
  servoIFV.write(SERVO_IFV_ABAJO);
  servoITV.write(SERVO_ITV_ABAJO);
  servoDFV.write(SERVO_DFV_ARRIBA);
  servoDTV.write(SERVO_DTV_ARRIBA);
  delay(tiempocorto);
  servoDFH.write(SERVO_DFH_MEDIO);
  servoDTH.write(SERVO_DTH_MEDIO);
  delay(tiempocorto);
  servoIM.write(SERVO_IM_ARRIBA);
  servoDFV.write(SERVO_DFV_ABAJO);
  servoDTV.write(SERVO_DTV_ABAJO);
  return;
}

void izqretrocede() {
  servoIFV.write(SERVO_IFV_ARRIBA);
  servoITV.write(SERVO_ITV_ARRIBA);
  servoDM.write(SERVO_DM_ABAJO);
  delay(tiempocorto);
  servoIFH.write(SERVO_IFH_ATRAS);
  servoITH.write(SERVO_ITH_MEDIO);
  delay(tiempocorto);
  servoIFV.write(SERVO_IFV_ABAJO);
  servoITV.write(SERVO_ITV_ABAJO);
  delay(tiempocorto);
  servoDFV.write(SERVO_DFV_ARRIBA);
  servoDTV.write(SERVO_DTV_ARRIBA);  
  servoIM.write(SERVO_IM_ARRIBA);
  delay(tiempocorto);
  servoIFH.write(SERVO_IFH_MEDIO);
  servoITH.write(SERVO_ITH_ATRAS);
  delay(tiempocorto);
  servoIM.write(SERVO_IM_ABAJO);
  servoDFV.write(SERVO_DFV_ABAJO);
  servoDTV.write(SERVO_DTV_ABAJO);
  servoDM.write(SERVO_DM_ARRIBA);
  return;
}

void derretrocede() {
  servoDFV.write(SERVO_DFV_ARRIBA);
  servoDTV.write(SERVO_DTV_ARRIBA);
  servoIM.write(SERVO_IM_ABAJO);
  delay(tiempocorto);
  servoDFH.write(SERVO_DFH_ATRAS);
  servoDTH.write(SERVO_DTH_MEDIO);
  delay(tiempocorto);
  servoDFV.write(SERVO_DFV_ABAJO);
  servoDTV.write(SERVO_DTV_ABAJO);
  delay(tiempocorto);
  servoIFV.write(SERVO_IFV_ARRIBA);
  servoITV.write(SERVO_ITV_ARRIBA);  
  servoDM.write(SERVO_DM_ARRIBA);
  delay(tiempocorto);
  servoDFH.write(SERVO_DFH_MEDIO);
  servoDTH.write(SERVO_DTH_ATRAS);
  delay(tiempocorto);
  servoDM.write(SERVO_DM_ABAJO);
  servoIFV.write(SERVO_IFV_ABAJO);
  servoITV.write(SERVO_ITV_ABAJO);
  servoIM.write(SERVO_IM_ARRIBA);
  return;
}

void funcionmano(){
  if(manoderecha == 'A') {
          funcionrobot='A';
    if(manoizquierda == 'C') {
      funcionrobot='A';
    }
    if(manoizquierda == 'B') {
      funcionrobot='I';
    }
    if(manoizquierda == 'P') {
      funcionrobot='I';
    }
  }
  if(manoderecha == 'R') {
          funcionrobot='R';
    if(manoizquierda == 'B') {
      funcionrobot='R';
    }
    if(manoizquierda == 'C') {
      funcionrobot='D';
    }
    if(manoizquierda == 'P') {
      funcionrobot='D';
    }
  } 
  if(manoderecha == 'S') {
    if(manoizquierda == 'B') {
      funcionrobot='I';
    }
    if(manoizquierda == 'C') {
      funcionrobot='D';
    }
    if(manoizquierda == 'P') {
      funcionrobot='U';
    }
  } 
  return;
}


void setup() {
  Serial.begin(9600);   //Inicia el puerto serie
  radio.begin();        // Inicializa la radio
  /**
   * Potencia de la radio
   * RF24_PA_LOW: Mínimo.
   */
  radio.setPALevel(RF24_PA_LOW);
  /**
   * Una tubería solo puede abrirse para leer o escribir, no
   * se puede leer y escribir en la misma tubería. Para ello
   * debes crear más tuberías.
   */
  radio.openReadingPipe(1, tuberia1);

  /**
   * Para recibir paquetes es necesario poner la radio a
   * escuchar en las tuberías registradas.
   */
  radio.startListening();
  
  //DIGITALES
  servoDFV.attach(17);
  servoDFH.attach(18);
  servoDM.attach(16);
  servoDTV.attach(14);
  servoDTH.attach(15);
  //ANALÓGICOS

  servoIFV.attach(4);   
  servoIFH.attach(3);
  servoIM.attach(5);
  servoITV.attach(7);
  servoITH.attach(6);
  izqdescansa();
  derdescansa();
  delay(2000);
}


void loop() {
  // Si hay bytes disponibles para leer en el puerto serie
  while(Serial.available()> 0 ){
    funcionrobot=Serial.read();
  }
  // Si hay bytes disponibles para leer en la radio
  while(radio.available() > 0) {
    /**
     * Lee el caracter de la radio. Donde dice "&lecturaRadio"
     * ponemos la variable donde vayamos a guardar el dato,
     * siempre precedido del símbolo "&". Donde dice
     * "sizeof(char)" le decimos el tamaño del dato que
     * esperamos, en este caso el tamaño de un "char". Esto
     * se le indica poniendo el tipo de dato de nuestra
     * variable dentro de "sizeof()".
     */
    radio.read(&lecturaRadio, sizeof(char));
    if(lecturaRadio=='R') {
      manoderecha='R';
    }
    if(lecturaRadio=='A') {
      manoderecha='A';
    }
    if(lecturaRadio=='S') {
      manoderecha='S';
    }
    if(lecturaRadio=='B') {
      manoizquierda='B';
    }
    if(lecturaRadio=='C') {
      manoizquierda='C';
    }
    if(lecturaRadio=='P') {
      manoizquierda='P';
    }
  }
  funcionmano();  
  switch (funcionrobot) {
    //AVANZA
    case 'A':
      izqavanza();
      deravanza();

    break;
    //RETROCEDE
    case 'R':
      izqretrocede();
      derretrocede();

    break;
    //IZQUIERDA
    case 'I':
      izqretrocede();
      deravanza();

    break;
    //DERECHA
    case 'D':
      izqavanza();
      derretrocede();

    break;
    default:
      izqdescansa();
      derdescansa();

    break;
    }
  manoderecha=0;
  manoizquierda=0;
  lecturaRadio=0;
  funcionrobot=0;
  delay(100);
}
