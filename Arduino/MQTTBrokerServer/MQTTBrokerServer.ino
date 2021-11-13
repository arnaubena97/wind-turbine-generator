#include <Wire.h>
#include <Adafruit_MCP23X17.h>

//Global variables
Adafruit_MCP23X17 mcp;
int serial = 0;

void setup() {  
  Serial.begin(9600);
  // Initialize the MCP23017
  if (!mcp.begin_I2C()) { // Try to connect to MCP
    Serial.println("Error.");
    while (1);
  }
  //Pins configurations
  mcp.pinMode(0, OUTPUT);
  mcp.pinMode(1, OUTPUT);
  mcp.pinMode(2, OUTPUT);
  mcp.pinMode(3, OUTPUT);
  mcp.pinMode(4, OUTPUT);
}

void loop() {
  // variabe will change every loop
  if(serial==0){
    mcp.digitalWrite(0, LOW);
    mcp.digitalWrite(1, LOW);
    mcp.digitalWrite(2, LOW);
    mcp.digitalWrite(3, LOW);
    mcp.digitalWrite(4, LOW);
    }
    else if(serial==1){
    mcp.digitalWrite(0, HIGH);
    mcp.digitalWrite(1, LOW);
    mcp.digitalWrite(2, LOW);
    mcp.digitalWrite(3, LOW);
    mcp.digitalWrite(4, LOW);
    }
    else if(serial==2){
    mcp.digitalWrite(0, HIGH);
    mcp.digitalWrite(1, HIGH);
    mcp.digitalWrite(2, LOW);
    mcp.digitalWrite(3, LOW);
    mcp.digitalWrite(4, LOW);
    }
    else if(serial==3){
    mcp.digitalWrite(0, HIGH);
    mcp.digitalWrite(1, HIGH);
    mcp.digitalWrite(2, HIGH);
    mcp.digitalWrite(3, LOW);
    mcp.digitalWrite(4, LOW);
    }
    else if(serial==4){
    mcp.digitalWrite(0, HIGH);
    mcp.digitalWrite(1, HIGH);
    mcp.digitalWrite(2, HIGH);
    mcp.digitalWrite(3, HIGH);
    mcp.digitalWrite(4, LOW);
    }
    else if(serial==5){
    mcp.digitalWrite(0, HIGH);
    mcp.digitalWrite(1, HIGH);
    mcp.digitalWrite(2, HIGH);
    mcp.digitalWrite(3, HIGH);
    mcp.digitalWrite(4, HIGH);
    }
  delay(500);

  
  /* BLINK EXAMPLE
  // Switch LED on
  mcp.digitalWrite(0, HIGH);
  
  delay(500);
  
  // Switch LED off
  mcp.digitalWrite(0, LOW);
  
  */
}
