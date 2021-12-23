/*
 * SLAVE CONFIG
 */

#include "Wire.h"

#define SLAVE_ADDR 0x04

uint8_t value1, value2;

char string1[20] = "DatProd1.1";

typedef struct {
  uint8_t humidity;
  uint8_t temperature[8];
  char name_id[3];

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
if(value1 ==0){
   Wire.write((byte *) &d1, sizeof(d1)); //float =4 -> 2 float = 4*2 =8
     Serial.print("Temp:");
  Serial.print(d1.temperature[7]);
  Serial.print("  Hum:");
  Serial.print(d1.humidity);
    Serial.print("  size:");
  Serial.print(sizeof(d1));
  Serial.println("   DHT Sended OK");
  cnt ++;
  d1.temperature[0] += (uint8_t)+1;
  d1.temperature[1] += (uint8_t)+1;
  d1.temperature[2] += (uint8_t)+1;
  d1.temperature[3] += (uint8_t)+1;
  d1.temperature[4] += (uint8_t)-1;
  d1.temperature[5] += (uint8_t)-1;
  d1.temperature[6] += (uint8_t)-1;
  d1.temperature[7] += (uint8_t)-1;
  d1.humidity = (uint8_t)(d1.humidity + 1);
}
  }
  
void setup() {
  d1.temperature[0] = (uint8_t)-3;
  d1.temperature[1] = (uint8_t)6;
  d1.temperature[2] = (uint8_t)13;
  d1.temperature[3] = (uint8_t)14;
  d1.temperature[4] = (uint8_t)20;
  d1.temperature[5] = (uint8_t)24;
  d1.temperature[6] = (uint8_t)30;
  d1.temperature[7] = (uint8_t)36;
  d1.humidity = (uint8_t)1;
  //d1.name_id = "Prova1 ";
  strcpy(d1.name_id , "W1");
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
