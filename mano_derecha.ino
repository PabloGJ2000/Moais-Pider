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

char mano_DERecha_mensaje = 0;

int sensorflexibleCORAZONDER=0;
int sensorflexiblePULGARDER=0;

const int dedopulgarDER=30;
const int dedocorazonDER=150;

int manoDERavanza=0;
int manoDERretrocede=0;
int manoDERparado=0;

int mano_DER_parado() {
  sensorflexibleCORAZONDER = analogRead(A4);
  sensorflexiblePULGARDER = analogRead(A3);
  if(sensorflexibleCORAZONDER > dedocorazonDER ) {
    manoDERparado=1;
  }
  return 0;
}
int mano_DER_retrocede() {
  sensorflexibleCORAZONDER = analogRead(A4);
  sensorflexiblePULGARDER = analogRead(A3);
  if(sensorflexiblePULGARDER > dedopulgarDER) {
    manoDERretrocede=1;
  }
  return 0;
}
int mano_DER_avanza () {
  sensorflexibleCORAZONDER = analogRead(A4);
  sensorflexiblePULGARDER = analogRead(A3);
  if((sensorflexibleCORAZONDER < dedocorazonDER )&(sensorflexiblePULGARDER < dedopulgarDER)) {
    manoDERavanza=1;
  }
  return 0;
}
void setup() {
  Serial.begin(9600);
  radio.begin(); // Inicializa la radio

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
  sensorflexibleCORAZONDER = analogRead(A4);
  sensorflexiblePULGARDER = analogRead(A3);
  mano_DER_parado();
  mano_DER_retrocede();
  mano_DER_avanza();
  if(manoDERretrocede==1) {
    mano_DERecha_mensaje='R';
  }
  if(manoDERavanza==1) {
    mano_DERecha_mensaje='A';
  }
  if(manoDERparado==1) {
    mano_DERecha_mensaje='S';
  }
  radio.write(&mano_DERecha_mensaje, sizeof(char));
  Serial.println(sensorflexiblePULGARDER);
  manoDERparado=0;
  manoDERretrocede=0;
  manoDERavanza=0;
  delay(5);
}
