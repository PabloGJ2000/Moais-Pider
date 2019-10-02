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

char mano_IZQecha_mensaje = 0;

int sensorflexibleCORAZONIZQ=0;
int sensorflexiblePULGARIZQ=0;

const int dedopulgarIZQ=100;
const int dedocorazonIZQ=130;

int manoIZQavanza=0;
int manoIZQretrocede=0;
int manoIZQparado=0;

int mano_IZQ_parado() {
  sensorflexibleCORAZONIZQ = analogRead(A4);
  sensorflexiblePULGARIZQ = analogRead(A3);
  if(sensorflexibleCORAZONIZQ > dedocorazonIZQ ) {
    manoIZQparado=1;
  }
  return 0;
}
int mano_IZQ_retrocede() {
  sensorflexibleCORAZONIZQ = analogRead(A4);
  sensorflexiblePULGARIZQ = analogRead(A3);
  if(sensorflexiblePULGARIZQ > dedopulgarIZQ) {
    manoIZQretrocede=1;
  }
  return 0;
}
int mano_IZQ_avanza () {
  sensorflexibleCORAZONIZQ = analogRead(A4);
  sensorflexiblePULGARIZQ = analogRead(A3);
  if((sensorflexibleCORAZONIZQ < dedocorazonIZQ )&(sensorflexiblePULGARIZQ < dedopulgarIZQ)) {
    manoIZQavanza=1;
  }
  return 0;
}
void setup() {
  Serial.begin(9600);
  radio.begin(); // Inicializa la radio
  Serial.println("hola");
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
  radio.openWritingPipe(tuberia1);
}

void loop() {
  sensorflexibleCORAZONIZQ = analogRead(A4);
  sensorflexiblePULGARIZQ = analogRead(A3);
  mano_IZQ_parado();
  mano_IZQ_retrocede();
  mano_IZQ_avanza();
  if(manoIZQretrocede==1) {
    mano_IZQecha_mensaje='B';
  }
  if(manoIZQavanza==1) {
    mano_IZQecha_mensaje='C';
  }
  if(manoIZQparado==1) {
    mano_IZQecha_mensaje='P';
  }

  radio.write(&mano_IZQecha_mensaje, sizeof(char));

  //Serial.println(mano_IZQecha_mensaje);
  Serial.println(sensorflexibleCORAZONIZQ);
  manoIZQparado=0;
  manoIZQretrocede=0;
  manoIZQavanza=0;
  delay(5);
}
