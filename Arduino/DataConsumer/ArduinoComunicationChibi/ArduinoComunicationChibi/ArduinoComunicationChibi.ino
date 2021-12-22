/*
 * SLAVE CONFIG
 */

#include "Wire.h"

#define SLAVE_ADDR 0x04

uint8_t value1, value2;

char string1[20] = "DatProd1.1";

typedef struct {
  uint8_t humidity;
  uint8_t temperature;
  char name_id[8];
} dht11;

dht11 d1;

int cnt = 1;
void receiveFunc(){
  //while(Wire.available()!=1);
    value1=(uint8_t)Wire.read();
    //value2=(uint8_t)Wire.read();
    Serial.print("Request data1: ");
    Serial.println(value1);

}

void sendRealdt1() {
  //Wire.write((byte *) &data1, 3* sizeof(dtp1));
  //Serial.println(3* sizeof(dtp1));
  //Wire.write((byte *) &d1, sizeof(dht11));
  Wire.write((byte *) &d1, sizeof(dht11)); //float =4 -> 2 float = 4*2 =8
  //Wire.write(string1, 20);
  //Wire.write((byte *) &cnt, 4);
  Serial.print("Temp:");
  Serial.print(d1.temperature);
  Serial.print("  Hum:");
  Serial.print(d1.humidity);
    Serial.print("  size:");
  Serial.print(sizeof(string1));
  Serial.println("   DHT Sended OK");
  cnt ++;
   d1.temperature += (uint8_t)1;
  d1.humidity = (uint8_t)(d1.humidity + 1);
  }
  
void setup() {
  d1.temperature = (uint8_t)24;
  d1.humidity = (uint8_t)1;
  //d1.name_id = "Prova1 ";
  strcpy(d1.name_id , "DP1-L1");
  Serial.begin(9600);
  Serial.println("Inicialize wire");
  Wire.begin(SLAVE_ADDR);
  Serial.println("Wire on adress 4");
    Wire.onReceive(receiveFunc);
   Wire.onRequest(sendRealdt1);


}

void loop() {
  // put your main code here, to run repeatedly:
  
}
