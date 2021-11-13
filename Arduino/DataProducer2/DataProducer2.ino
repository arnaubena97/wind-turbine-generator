#include <Wire.h>
#include <LiquidCrystal_I2C.h> // Library for LCD

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); 

void setup() {

    //Inicialitzem el serial port
    //Serial.begin(9600);
  
    // Inicialitzem el LCD display:
    lcd.init();
    lcd.backlight();
    
    // Inicialitzem el ADXL345:
    Wire.begin();
    
    Wire.beginTransmission(0x53);
    Wire.write(0x2C); 
    Wire.write(0x08); 
    Wire.endTransmission();
    
    Wire.beginTransmission(0x53);
    Wire.write(0x31); 
    Wire.write(0x08); 
    Wire.endTransmission();
    
    Wire.beginTransmission(0x53);
    Wire.write(0x2D); 
    Wire.write(0x08); 
    Wire.endTransmission();
}

void loop() {
    //Llegim la X, als registres 0x32 i 0x33
    Wire.beginTransmission(0x53);
    Wire.write(0x32); 
    Wire.endTransmission();
    Wire.requestFrom(0x53, 1);
    byte x0 = Wire.read();
    
    Wire.beginTransmission(0x53);
    Wire.write(0x33); 
    Wire.endTransmission();
    Wire.requestFrom(0x53, 1);
    byte x1 = Wire.read();
    x1 = x1 & 0x03;
    
    uint16_t x = (x1 << 8) + x0;
    int16_t xf = x;
    if(xf > 511){
      xf = xf - 1024;
    }
    float xa = xf * 0.004;
    

    //Llegim la Y, als registres 0x34 i 0x35
    Wire.beginTransmission(0x53);
    Wire.write(0x34); 
    Wire.endTransmission();
    Wire.requestFrom(0x53, 1);
    byte y0 = Wire.read();
    
    Wire.beginTransmission(0x53);
    Wire.write(0x35); 
    Wire.endTransmission();
    Wire.requestFrom(0x53, 1);
    byte y1 = Wire.read();
    y1 = y1 & 0x03;
    
    uint16_t y = (y1 << 8) + y0;
    int16_t yf = y;
    if(yf > 511){
      yf = yf - 1024;
    }
    float ya = yf * 0.004;

    
    //Llegim la Z, als registres 0x36 i 0x37
    Wire.beginTransmission(0x53);
    Wire.write(0x36); 
    Wire.endTransmission();
    Wire.requestFrom(0x53, 1);
    byte z0 = Wire.read();
    
    Wire.beginTransmission(0x53);
    Wire.write(0x37); 
    Wire.endTransmission();
    Wire.requestFrom(0x53, 1);
    byte z1 = Wire.read();
    z1 = z1 & 0x03;
    
    uint16_t z = (z1 << 8) + z0;
    int16_t zf = z;
    if(zf > 511){
      zf = zf - 1024;
    }
    
    float za = zf * 0.004;
    

    //Creem els Strings
    char prova[] = "arduino";

    //Serial.println(xa);
    //Serial.println(ya);
    //Serial.println(za);
    
    // Printem les dades al LCD:
    //X
    lcd.setCursor(1, 0); //Posem el cursor a la primera fila a la tercera columna(les columnes començen pel 0).
    lcd.print("X"); // Escribim la X"
    lcd.setCursor(2, 0);
    lcd.print("=");
    lcd.setCursor(3, 0);
    lcd.print(xa);

    //Y
    lcd.setCursor(8, 0); //Posem el cursor a la primera fila a la 8 columna(les columnes començen pel 0).
    lcd.print("Y");
    lcd.setCursor(9, 0);
    lcd.print("=");
    lcd.setCursor(10, 0);
    lcd.print(ya); // Escribim la Y"

    //Z
    lcd.setCursor(1, 1); //Posem el cursor a la segona fila a la tercera columna(les columnes començen pel 0).
    lcd.print("Z");
    lcd.setCursor(2, 1);
    lcd.print("=");
    lcd.setCursor(3, 1);
    lcd.print(za); // Escribim la Z"

    delay(1000);
}