#include <SoftwareSerial.h>

const byte rxPin = 2;
const byte txPin = 3;

// configura un nuevo objeto serial
SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin );

void setup ( )  {
  // definir modos de pin para tx, rx:
  pinMode ( rxPin , INPUT ) ;
  pinMode ( txPin , OUTPUT ) ;
  // establece la velocidad de datos para el puerto de SoftwareSerial
  mySerial.begin (9600 ) ;
  Serial.begin(9600);
} 

void loop(){
  String message;
  message = Serial.readString();

  mySerial.print(message);
  
  for(int i = 0; i < message.length(); i++){
    Serial.print(message[i]);

  }
  
  mySerial.print("\n");
  
  
  delay(1000);
}
