/*
 * SLAVE CONFIG
 */

#include "Wire.h"

#define SLAVE_ADDR 0x04
uint8_t whatDP;

typedef struct {
  uint8_t humidity;
  uint8_t temperature[8];
  char name_id[3];

} dht11;
dht11 d1;

typedef struct {
  uint8_t x_axis;
  uint8_t y_axis;
  uint8_t z_axis;
  char name_id[3];

} adxl345;
adxl345 d2;

void receiveFunc(){
  //while(Wire.available()!=1);
    whatDP=(uint8_t)Wire.read();
    //value2=(uint8_t)Wire.read();
    Serial.print("Request data1: ");
    Serial.println((uint8_t)whatDP);

}

void sendRealdt1() {
  if(whatDP ==0){
     Wire.write((byte *) &d1, sizeof(d1)); //float =4 -> 2 float = 4*2 =8
    Serial.println("DHT Sended OK");
    d1.temperature[0] += (uint8_t)+1;
    d1.temperature[1] += (uint8_t)+1;
    d1.temperature[2] += (uint8_t)+1;
    d1.temperature[3] += (uint8_t)+1;
    d1.temperature[4] += (uint8_t)-1;
    d1.temperature[5] += (uint8_t)-1;
    d1.temperature[6] += (uint8_t)-1;
    d1.temperature[7] += (uint8_t)-1;
    d1.humidity = (uint8_t)(d1.humidity + 1);
  }else if (whatDP==1){
     Wire.write((byte *) &d2, sizeof(d2));
     Serial.println("ADXL Sended OK");
     d2.x_axis += 1;
     d2.y_axis += 1;
     d2.z_axis += 1;
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
  d2.x_axis = 0;
  d2.y_axis = 1;
  d2.z_axis = 2;
  //d1.name_id = "Prova1 ";
  strcpy(d1.name_id , "W1");
  strcpy(d2.name_id , "W1");
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
