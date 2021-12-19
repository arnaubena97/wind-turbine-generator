/*
 * SLAVE CONFIG
 */

#include <Wire.h>

#define SLAVE_ADDR 0x04

uint8_t value1, value2;

typedef struct {
  float temperature;
  float humidity;
} dht11;

dht11 d1;
int cnt = 0;
void receiveFunc(){
    value1=(uint8_t)Wire.read();
    Serial.print("Request data: ");
    Serial.println(value1);
}

void sendRealdt1() {
  //Wire.write((byte *) &data1, 3* sizeof(dtp1));
  //Serial.println(3* sizeof(dtp1));
  //Wire.write((byte *) &d1, sizeof(dht11));
  //Wire.write((byte *) &d1, 8); //float =4 -> 2 float = 4*2 =8
  Wire.write((byte *) &cnt, 4);
  Serial.print("Temp:");
  Serial.print(d1.temperature);
  Serial.print("  Hum:");
  Serial.print(d1.humidity);
    Serial.print("  size:");
  Serial.print(cnt);
  Serial.println("   DHT Sended OK");
  cnt ++;
  }
  
void setup() {
  // put your setup code here, to run once:
  d1.temperature = 24.3;
  d1.humidity = 88.4;
  
  Serial.begin(9600);
  Serial.println("Inicialize wire");
  Wire.begin(SLAVE_ADDR);
  Serial.println("Wire on adress 4");
    Wire.onRequest(sendRealdt1);
  Wire.onReceive(receiveFunc);

}

void loop() {
  // put your main code here, to run repeatedly:
  
}
