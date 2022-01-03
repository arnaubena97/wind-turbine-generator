
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

void printadxl(){
  int q1 = (d2.x_axis);
  int q2 = (d2.y_axis);
  int q3 = (d2.z_axis);
     Serial.print("ADXL- Name: ");
     Serial.print(d2.name_id);
     Serial.print(" x: ");
     Serial.print(q1);
     Serial.print(" y: ");
     Serial.print(q2);
     Serial.print(" z: ");
     Serial.println(q3);
     
}

void printdht(){
     Serial.print("DHT- Name: ");
     Serial.print(d1.name_id);
     Serial.print(" T: ");
     Serial.print(d1.temperature[7]);
     Serial.print(" H: ");
     Serial.print(d1.humidity);
     Serial.print(" t6: ");
     Serial.print(d1.temperature[6]);
     Serial.print(" t5: ");
     Serial.print(d1.temperature[5]);
     Serial.print(" t4: ");
     Serial.print(d1.temperature[4]);
     Serial.print(" t3: ");
     Serial.print(d1.temperature[3]);
     Serial.print(" t2: ");
     Serial.print(d1.temperature[2]);
     Serial.print(" t1: ");
     Serial.print(d1.temperature[1]);
     Serial.print(" t0: ");
     Serial.println(d1.temperature[0]);
     
}


void receiveFunc(){
    whatDP=(uint8_t)Wire.read();
    Serial.print("Request data1: ");
    Serial.println((uint8_t)whatDP);

}

void sendRealdt1() {
  if(whatDP ==0){
     Wire.write((byte *) &d1, sizeof(d1)); //float =4 -> 2 float = 4*2 =8
    Serial.println("DHT Sended OK");

  }else if (whatDP==1){
     Wire.write((byte *) &d2, sizeof(d2));
     Serial.print("ADXL Sended OK  -  ");
     //printdht();
     
  }
}
  
void setup() {
  d1.temperature[0] = 0;
  d1.temperature[1] = 0;
  d1.temperature[2] = 0;
  d1.temperature[3] = 0;
  d1.temperature[4] = 0;
  d1.temperature[5] = 0;
  d1.temperature[6] = 0;
  d1.temperature[7] = 0;
  d1.humidity = 01;
  d2.x_axis = 0;
  d2.y_axis = 0;
  d2.z_axis = 0;
  //d1.name_id = "Prova1 ";
  //strcpy(d1.name_id , "W1");
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
    getValue(message, ';', 1).toCharArray(d2.name_id, 3);
    d2.x_axis = (uint8_t) getValue(message, ';', 2).toInt();
    d2.y_axis = (uint8_t) getValue(message, ';', 3).toInt();
    d2.z_axis = (uint8_t) getValue(message, ';', 4).toInt();
    Serial.print("ADXL Readed OK  -  ");
    printadxl();
  }else if(sensors.equals("dht")){
    getValue(message, ';', 1).toCharArray(d1.name_id, 3);
    d1.humidity = (uint8_t) getValue(message, ';', 2).toInt() ;
    d1.temperature[0] = d1.temperature[1];
    d1.temperature[1] = d1.temperature[2];
    d1.temperature[2] = d1.temperature[3];
    d1.temperature[3] = d1.temperature[4];
    d1.temperature[4] = d1.temperature[5];
    d1.temperature[5] = d1.temperature[6];
    d1.temperature[6] = d1.temperature[7];
    d1.temperature[7] = (uint8_t) getValue(message, ';', 3).toInt();
    Serial.print("DHT Readed OK  -  ");
    printdht();
  }
}

void loop(){
  delay(100);
  readESP();
  
  delay(100);
  
  delay(2000); 
}
