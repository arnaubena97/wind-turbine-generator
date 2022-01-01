#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//wifi
const char* ssid = "ssidq";
const char* password = "passwordq";
const char* mqtt_server = "192.168.4.1";
//variables globals del callback
/*
byte* buffer;
boolean Rflag=false;
int r_len;
*/

//byte var;
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
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
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

String messageDht;
String messageAdxl;
void callback(char* topic, byte* payload, unsigned int length) {
  messageAdxl = "adxl;";
  messageDht = "dht;";
  String first = getValue(topic, '/', 0);
  String last = getValue(topic, '/', 2);
  if( first.equals("turbines")){
    if(last.equals("adxl345")){
      for (int i = 0; i < length; i++) {
        messageAdxl.concat((char) payload[i]);
          
      }
      for (int i = 0; i < messageAdxl.length(); i++){
    Serial.print(messageAdxl[i]); 
  }
    }
    else if(last.equals("dht11")){
      for (int i = 0; i < length; i++) {
        messageDht.concat((char) payload[i]);
        
      }
        for (int i = 0; i < messageDht.length(); i++){
    Serial.print(messageDht[i]); 
  }
    }
  }
  
  Serial.println();

}


void reconnect() {
 // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic", "hello world");
      // ... and resubscribe
        client.subscribe("turbines/+/adxl345");
  client.subscribe("turbines/+/dht11");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(1000);
    }
}
}

void setup() {
  //wifi
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  //client.subscribe("#");
  client.subscribe("turbines/+/adxl345");
  client.subscribe("turbines/+/dht11");
  delay(100);

}

void loop() {
   if (!client.connected()) {
        reconnect();
     }
  delay(500);
  client.loop();   
}
