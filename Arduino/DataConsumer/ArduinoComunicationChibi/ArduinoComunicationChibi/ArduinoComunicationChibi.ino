/*
 * SLAVE CONFIG
 */

#include "Wire.h"

#define SLAVE_ADDR 0x04

uint8_t value1, value2;

typedef struct {
  float temperature;
  float humidity;
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
  //Wire.write((byte *) &d1, 8); //float =4 -> 2 float = 4*2 =8
  Wire.write((byte *) &cnt, sizeof(cnt));
  Serial.print("Temp:");
  Serial.print(d1.temperature);
  Serial.print("  Hum:");
  Serial.print(d1.humidity);
    Serial.print("  cnt:");
  Serial.print(cnt);
  Serial.println("   DHT Sended OK");
  cnt ++;
  }
  
void setup() {
  d1.temperature = 24.3;
  d1.humidity = 88.4;
  
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
