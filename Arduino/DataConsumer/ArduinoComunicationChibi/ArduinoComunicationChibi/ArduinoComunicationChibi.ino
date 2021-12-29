
#include "Wire.h"

#define SLAVE_ADDR 0x04
uint8_t whatDP;
String message = "";


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
    whatDP=(uint8_t)Wire.read();
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
     Serial.print("ADXL- Name: ");
     Serial.print(d2.name_id);
     Serial.print(" x: ");
     Serial.print(d2.x_axis);
     Serial.print(" y: ");
     Serial.print(d2.y_axis);
     Serial.print(" z: ");
     Serial.print(d2.z_axis);
     Serial.println("  ADXL Sended OK");
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
  d2.y_axis = 0;
  d2.z_axis = 0;
  //d1.name_id = "Prova1 ";
  strcpy(d1.name_id , "W1");
  //strcpy(d2.name_id , "W1");
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println("Inicialize wire");
  Wire.begin(SLAVE_ADDR);
  Serial.println("Wire on adress 4");
  Wire.onReceive(receiveFunc);
  Wire.onRequest(sendRealdt1);
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}



void readESP(){
  message = Serial1.readString();
  message = getValue(message, '=', 0);
  String sensors = getValue(message, ';', 0);
  for(int i = 0; i < message.length(); i++){
    Serial.print(message[i]);
  }
  if(sensors.equals("adxl")){
    d2.x_axis = (uint8_t) getValue(message, ';', 2).toInt() ;
    d2.y_axis = (uint8_t) getValue(message, ';', 3).toInt()  ;
    d2.z_axis = (uint8_t) getValue(message, ';', 4).toInt()  ;
    getValue(message, ';', 1).toCharArray(d2.name_id, 3);
  }else if(sensors.equals("dht11")){
    int valuew = 3;
  }
  
}

void loop(){
  delay(100);
  readESP();
  delay(100);
  
  Serial.print("\n");
  delay(3000);  
}
