#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // set LCD address, number of columns and rows
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);// assignar unic id

void setup() {
    Wire.begin(0,2);
    // Inicialitzem el LCD display:
    lcd.begin();
    lcd.backlight();
    // Inicialitzem el ADXL345:
    accel.begin();
}

void loop() {

    sensors_event_t accelEvent;  
    accel.getEvent(&accelEvent);

    // Printem les dades al LCD:
    writeLCD("X", correctValueX(accelEvent.acceleration.x), 0, 0);
    writeLCD("Y", correctValueY(accelEvent.acceleration.y), 8, 0);
    writeLCD("Z", correctValueZ(accelEvent.acceleration.z), 0, 1);
    delay(500);
}


String correctValueX(float value){
  float maxi = 10.47;
  float minu = -10.08;
  float refmax = 9.8;
  float refmin = -9.8;
  float range = maxi - minu;
  float rangeref = refmax - refmin;
  float correctedValue = (((value - minu) * rangeref) / range) + refmin;
  String stringOne = String(correctedValue, 2);
  return stringOne;
}

String correctValueY(float value){
  float maxi = 10.12;
  float minu = -10.32;
  float refmax = 9.8;
  float refmin = -9.8;
  float range = maxi - minu;
  float rangeref = refmax - refmin;
  float correctedValue = (((value - minu) * rangeref) / range) + refmin;
  String stringOne = String(correctedValue, 2);
  return stringOne;
}

String correctValueZ(float value){
  float maxi = 9.28;
  float minu = -10.40;
  float refmax = 9.8;
  float refmin = -9.8;
  float range = maxi - minu;
  float rangeref = refmax - refmin;
  float correctedValue = (((value - minu) * rangeref) / range) + refmin;
  String stringOne = String(correctedValue, 2);
  return stringOne;
}

void writeLCD(String axis, String value, int posx, int posy){
      lcd.setCursor(posx, posy); 
      lcd.print(axis); 
      lcd.setCursor(posx+1, posy);
      lcd.print("=");
      lcd.setCursor(posx+2, posy);
      lcd.print(value);
    }
