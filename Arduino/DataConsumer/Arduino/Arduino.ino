#include <SoftwareSerial.h>



// configura un nuevo objeto serial
SoftwareSerial mySerial(2, 3);

void setup ( )  {

  mySerial.begin (4800);
  Serial.begin(9600);
} 

String message = "";
void readESP(){
  message = Serial.readString();
}

void loop(){
  delay(100);
  readESP();
  delay(100);
  
  for(int i = 0; i < message.length(); i++){
    Serial.print(message[i]);

  }
  Serial.print("\n");
  delay(1000);  
}
