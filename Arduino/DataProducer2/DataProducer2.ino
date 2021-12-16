#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // set LCD address, number of columns and rows
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);// assignar unic id

byte wifiChar3[] = { //wifi char
  B00000,
  B00000,
  B00000,
  B00111,
  B01000,
  B10011,
  B10100,
  B10101
};
byte wifiChar2[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00011,
  B00100,
  B00101
};
byte wifiChar1[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00001
};


//wifi
const char* ssid = "ssidq";
const char* password = "passwordq";
const char* mqtt_server = "192.168.4.1";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  //quit comment if not work
  //while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  //}

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
 // while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(1000);
    }
  //}
}





void setup() {
    Wire.begin(0,2);
    // Inicialitzem el LCD display:
    lcd.begin();
    lcd.backlight();
    // Inicialitzem el ADXL345:
    accel.begin();

    //wifi
    Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  lcd.createChar(0, wifiChar1);
  lcd.createChar(1, wifiChar2);
  lcd.createChar(2, wifiChar3);
}

void loop() {
    if (!client.connected()) {
      lcd.setCursor(15, 1); 
      lcd.print(" ");
        reconnect();
     }else{
       lcd.setCursor(15, 1); 
       lcd.print(" ");
       delay(100);
       lcd.setCursor(15, 1); 
       lcd.write(0);
       delay(100);
       lcd.setCursor(15, 1); 
       lcd.write(1);
       delay(100);
       lcd.setCursor(15, 1); 
       lcd.write(2);
       delay(200);
       lcd.setCursor(15, 1); 
       lcd.write(1);
       delay(100);
       lcd.setCursor(15, 1); 
       lcd.write(0);
     }
    client.loop();

    sensors_event_t accelEvent;  
    accel.getEvent(&accelEvent);
    String ax = correctValueX(accelEvent.acceleration.x);
    String ay = correctValueY(accelEvent.acceleration.y);
    String az = correctValueZ(accelEvent.acceleration.z);
    // Printem les dades al LCD:
    writeLCD("X", ax, 0, 0);
    writeLCD("Y", ay, 8, 0);
    writeLCD("Z", az, 0, 1);
    String mess = "X=" + ax + " Y="+ay+" Z="+az;
    mess.toCharArray(msg, 50);
    client.publish("broker/adxl345", msg);
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
