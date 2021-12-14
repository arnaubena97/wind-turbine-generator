
#include "DHT.h"

#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 1
#define maxX 127//159 
#define maxY 63 //127

bool writeerase = true;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(1200);///wait for the one second spalsh screen before anything is sent to the LCD.
  setHome();//set the cursor back to 0,0.
  clearScreen();//clear anything that may have been previously printed ot the screen.
  setHome();
  delay(10);
  dht.begin();
}



void loop() {
  
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) ) {
    delay(2000);
    return;
  }
  float hic = dht.computeHeatIndex(t, h, false);
  drawBoxes();
  printText(t, h, hic);
  //drawWifi();
  
  
  delay(1500);
}



void printText(float t, float h, float hic){
  setHome();
  setX(maxX/2-58);
  setY(maxY/4+2);
  Serial.print("Temperature:");
  Serial.print(t);
  Serial.print(" C");
  
  setHome();
  setX(7);
  setY(maxY/2+19);
  Serial.print("Hum:");
  Serial.print(int(h));
  Serial.print("%");
  
  setHome();
  setX(maxX/2-1);
  setY(maxY/2+19);
  Serial.print("HIC:");
  Serial.print(int(hic));
  Serial.print("%");

  
}
void drawBoxes(){
  drawBox(0,0,maxX, maxY/2, 0); //temperature box
  drawBox(1,1,maxX-1, maxY/2-1, 0);
  
  drawBox(0,maxY/2,maxX/2-5, maxY, 0); //humidity box
  drawBox(1,maxY/2+1,maxX/2-6, maxY-1, 0);
  
  drawBox(maxX/2-6,maxY/2,maxX-21, maxY, 0);//hic box
  drawBox(maxX/2-5,maxY/2+1,maxX-22, maxY-1, 0);

  drawBox(maxX-20,maxY/2,maxX, maxY, 0);//wifi box
  drawBox(maxX-21,maxY/2+1,maxX-1, maxY-1, 0);
}

void clearScreen()
{
  //clears the screen, you will use this a lot!
  Serial.write(0x7C);
  Serial.write((byte)0); //CTRL @
  //can't send LCD.write(0) or LCD.write(0x00) because it's interprestted as a NULL
}

void drawBox(byte x1, byte y1, byte x2, byte y2, byte set)
{
  //draws a box from two given points. You can set and reset just as the pixel function. 
  Serial.write(0x7C);
  Serial.write(0x0F);//CTRL o 
  Serial.write(x1);
  Serial.write(y1);
  Serial.write(x2);
  Serial.write(y2);
  //Serial.write(0x01);
  delay(10);
}

void setHome() {
  setX((byte)0);
  setY((byte)0);
}
void setY(byte posY)//0-63 or 0-127 pixels
{
  //Set the y position 
  Serial.write(0x7C);
  Serial.write(0x19);//CTRL y
  Serial.write(posY);
}
void setX(byte posX) //0-127 or 0-159 pixels
{
  //Set the X position 
  Serial.write(0x7C);
  Serial.write(0x18);//CTRL x
  Serial.write(posX);
  }

void setPixel(byte x, byte y)
{
  Serial.write(0x7C);
  Serial.write(0x10);//CTRL p
  Serial.write(x);
  Serial.write(y);
  Serial.write(0x01);

}

void W1(){
    //First dot
  setPixel(maxX-10, maxY/2 +3);
  setPixel(maxX-11, maxY/2 +3);
  setPixel(maxX-10, maxY/2 +4);
  setPixel(maxX-11, maxY/2 +4);
  delay(200);
}
void W2(){
  //second bar
  setPixel(maxX-10, maxY/2 +6);
  setPixel(maxX-11, maxY/2 +6);
  setPixel(maxX-10, maxY/2 +7);
  setPixel(maxX-11, maxY/2 +7);
  setPixel(maxX-9, maxY/2 +6);
  setPixel(maxX-9, maxY/2 +7);
  setPixel(maxX-12, maxY/2 +6);
  setPixel(maxX-12, maxY/2 +7);
  setPixel(maxX-13, maxY/2 +5);
  setPixel(maxX-8, maxY/2 +5);
  setPixel(maxX-13, maxY/2 +6);
  setPixel(maxX-8, maxY/2 +6);
  delay(200);
}
void W3(){
  //thrid bar
  setPixel(maxX-10, maxY/2 +10);
  setPixel(maxX-11, maxY/2 +10);
  setPixel(maxX-9, maxY/2 +10);
  setPixel(maxX-12, maxY/2 +10);
  setPixel(maxX-8, maxY/2 +10);
  setPixel(maxX-13, maxY/2 +10);
  
  setPixel(maxX-10, maxY/2 +11);
  setPixel(maxX-11, maxY/2 +11);
  setPixel(maxX-9, maxY/2 +11);
  setPixel(maxX-12, maxY/2 +11);
  setPixel(maxX-8, maxY/2 +11);
  setPixel(maxX-13, maxY/2 +11);

  setPixel(maxX-7, maxY/2 +10);
  setPixel(maxX-7, maxY/2 +9);
  setPixel(maxX-6, maxY/2 +9);
  setPixel(maxX-6, maxY/2 +8);

  setPixel(maxX-14, maxY/2 +10);
  setPixel(maxX-14, maxY/2 +9);
  setPixel(maxX-15, maxY/2 +9);
  setPixel(maxX-15, maxY/2 +8);
  delay(200);
}
void W4(){
//fourth bar
  setPixel(maxX-10, maxY/2 +14);
  setPixel(maxX-11, maxY/2 +14);
  setPixel(maxX-9, maxY/2 +14);
  setPixel(maxX-12, maxY/2 +14);
  setPixel(maxX-8, maxY/2 +14);
  setPixel(maxX-13, maxY/2 +14);
  setPixel(maxX-7, maxY/2 +14);
  setPixel(maxX-6, maxY/2 +14);
  setPixel(maxX-14, maxY/2 +14);
  setPixel(maxX-15, maxY/2 +14);
  
  setPixel(maxX-10, maxY/2 +15);
  setPixel(maxX-11, maxY/2 +15);
  setPixel(maxX-9, maxY/2 +15);
  setPixel(maxX-12, maxY/2 +15);
  setPixel(maxX-8, maxY/2 +15);
  setPixel(maxX-13, maxY/2 +15);
  setPixel(maxX-7, maxY/2 +15);
  setPixel(maxX-6, maxY/2 +15);
  setPixel(maxX-14, maxY/2 +15);
  setPixel(maxX-15, maxY/2 +15);

  setPixel(maxX-5, maxY/2 +14);
  setPixel(maxX-5, maxY/2 +13);
  setPixel(maxX-4, maxY/2 +13);
  setPixel(maxX-4, maxY/2 +12);
  setPixel(maxX-3, maxY/2 +12);
  setPixel(maxX-3, maxY/2 +11);

  setPixel(maxX-16, maxY/2 +14);
  setPixel(maxX-16, maxY/2 +13);
  setPixel(maxX-17, maxY/2 +13);
  setPixel(maxX-17, maxY/2 +12);
  setPixel(maxX-18, maxY/2 +12);
  setPixel(maxX-18, maxY/2 +11);

  delay(200);
}

void drawWifi(){
  W1();
  W2();
  W3();
  W4();
}
