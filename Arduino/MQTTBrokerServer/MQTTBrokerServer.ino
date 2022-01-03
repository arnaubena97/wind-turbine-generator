#include <Wire.h>
#include <Adafruit_MCP23X17.h>
#include <ESP8266WiFi.h>
#include "uMQTTBroker.h"


//Global variables
Adafruit_MCP23X17 mcp;
uMQTTBroker myBroker;

/*
 * Your WiFi config here
 */
char ssid[] = "ssidq";      // your network SSID (name)
char pass[] = "passwordq"; // your network password

const byte numPins = 6;
byte pins[] = {0, 1, 2, 3, 4, 5};
byte var;
void setup() { 
  Serial.begin(9600);
  WiFi.softAP(ssid, pass);
  Serial.println("AP started");
  Serial.println("IP address: " + WiFi.softAPIP().toString());
  
  Serial.println("Starting MQTT broker");
  myBroker.init();
  myBroker.subscribe("#");

  
  
  // Initialize the MCP23017
  if (!mcp.begin_I2C()) { // Try to connect to MCP
    Serial.println("Error.");
    while (1);
  }
  for (int i=0; i<numPins; i++) {
    mcp.pinMode(pins[i], OUTPUT);
  }
}

void loop() {
  
  // variabe will change every loop
  var = myBroker.getClientCount();
  //uint16_t vnum = myBroker.getClientCount();
  //Serial.print("Connections: ");
  //Serial.println(var);
  int varint= (int) var;
  Serial.print("Connections: ");
  Serial.println(varint);
  for (int q=0; q<numPins; q++) {
    if(q<varint){
      //Serial.print("PIN HIGH : ");
      //Serial.println(pins[q]);
      mcp.digitalWrite(pins[q], HIGH);
      
    }else{
      //Serial.print("PIN LOW : ");
      //Serial.println(pins[q]);
       mcp.digitalWrite(pins[q], LOW);
    }
    //delay(1200);
  }    
 
  delay(500);

}
