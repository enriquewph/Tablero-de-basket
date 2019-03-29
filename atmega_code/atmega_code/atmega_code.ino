#include <SoftwareSerial.h>

#define BT_RX_PIN 2
#define BT_TX_PIN 3
#define BT_STATE_PIN 4
#define POS_ROJO_PIN 7
#define POS_VERDE_PIN 8

#define SCK_PIN 13
#define MOSI_PIN 11
#define LATCH_PIN 10

SoftwareSerial btSerial(BT_RX_PIN, BT_TX_PIN); // RX, TX

unsigned int posesionRojo = 0;
unsigned int puntajeRojo = 0;
unsigned int puntajeVerde = 0;
unsigned int tiempo24 = 19;
unsigned int minutos = 12;
unsigned int segundos = 34;
unsigned int cuarto = 0;

void setup() {
  configurePins();
  delay(1000);
  Serial.begin(9600);
  btSerial.begin(38400);
  Serial.println("Encendido.");
  updateLeds();
}

void loop() {
  //Listo, conectado / por conectar
  if (btSerial.available()) //PUNTOSROJO|PUNTOSVERDE|TIMER24|MINUTOS|SEGUNDOS|PERIODO|POSESIONROJO
  {
    String inputString = btSerial.readStringUntil('\n');
    unsigned int del1 = inputString.indexOf('|');
    puntajeRojo = inputString.substring(0, del1).toInt();
    unsigned int del2 = inputString.indexOf('|', del1 + 1);
    puntajeVerde = inputString.substring(del1 + 1, del2).toInt();
    unsigned int del3 = inputString.indexOf('|', del2 + 1);
    tiempo24 = inputString.substring(del2 + 1, del3).toInt();
    unsigned int del4 = inputString.indexOf('|', del3 + 1);
    minutos = inputString.substring(del3 + 1, del4).toInt();
    unsigned int del5 = inputString.indexOf('|', del4 + 1);
    segundos = inputString.substring(del4 + 1, del5).toInt();
    unsigned int del6 = inputString.indexOf('|', del5 + 1);
    cuarto = inputString.substring(del5 + 1, del6).toInt();
    unsigned int del7 = inputString.indexOf('|', del6 + 1);
    posesionRojo = inputString.substring(del6 + 1, del7).toInt();
    updateLeds();
  }
}

byte codBCD[] = {63,6,91,79,102,109,125,7,127,103};

void updateLeds()
{

  shiftOut(MOSI_PIN, SCK_PIN, MSBFIRST, codBCD[decena(minutos)]);
  shiftOut(MOSI_PIN, SCK_PIN, MSBFIRST, codBCD[unidad(minutos)]);
  shiftOut(MOSI_PIN, SCK_PIN, MSBFIRST, codBCD[decena(segundos)]);
  shiftOut(MOSI_PIN, SCK_PIN, MSBFIRST, codBCD[unidad(segundos)]);
  shiftOut(MOSI_PIN, SCK_PIN, MSBFIRST, codBCD[decena(tiempo24)]);
  shiftOut(MOSI_PIN, SCK_PIN, MSBFIRST, codBCD[unidad(tiempo24)]);
  shiftOut(MOSI_PIN, SCK_PIN, MSBFIRST, codBCD[centena(puntajeRojo)]);
  shiftOut(MOSI_PIN, SCK_PIN, MSBFIRST, codBCD[decena(puntajeRojo)]);
  shiftOut(MOSI_PIN, SCK_PIN, MSBFIRST, codBCD[unidad(puntajeRojo)]);
  shiftOut(MOSI_PIN, SCK_PIN, MSBFIRST, codBCD[cuarto]);
  shiftOut(MOSI_PIN, SCK_PIN, MSBFIRST, codBCD[centena(puntajeVerde)]);
  shiftOut(MOSI_PIN, SCK_PIN, MSBFIRST, codBCD[decena(puntajeVerde)]);
  shiftOut(MOSI_PIN, SCK_PIN, MSBFIRST, codBCD[unidad(puntajeVerde)]);
  
  digitalWrite(POS_ROJO_PIN, posesionRojo);
  digitalWrite(POS_VERDE_PIN, !posesionRojo);
  
  digitalWrite(LATCH_PIN, HIGH);
  delay(100);
  digitalWrite(LATCH_PIN, LOW);
}

unsigned short unidad(int variable)
{
  short a = variable % 10;
  return (a);
}

unsigned short decena(int variable)
{
  short a = (variable % 100) / 10;
  return (a);
}

unsigned short centena(int variable)
{
  short a = variable / 100;
  return (a);
}

void configurePins()
{
  pinMode(BT_RX_PIN, INPUT);
  pinMode(BT_TX_PIN, OUTPUT);
  pinMode(BT_STATE_PIN, INPUT);
  pinMode(POS_ROJO_PIN, OUTPUT);
  pinMode(POS_VERDE_PIN, OUTPUT);
  pinMode(MOSI_PIN, OUTPUT);
  pinMode(SCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  
  digitalWrite(POS_ROJO_PIN, LOW);
  digitalWrite(POS_VERDE_PIN, LOW);
  digitalWrite(MOSI_PIN, LOW);
  digitalWrite(SCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);
}
