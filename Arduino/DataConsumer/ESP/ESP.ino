#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//wifi
const char* ssid = "ssid1";
const char* password = "password1";
const char* mqtt_server = "192.168.4.1";

const char* clientID = "ESP-01";
const char* clientUserName = "ESP-01";
const char* clientPassword = "ESP-01";

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


String message;
void callback(char* topic, byte* payload, unsigned int length) {
  message = "";
  for (int i = 0; i < length; i++) {
    message.concat((char) payload[i]);
    //Serial.print((char)payload[i]);
    
  }
  Serial.println();

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
      client.subscribe("#");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(1000);
    }
}

void setup() {
  //wifi
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  client.connect(clientID,clientUserName,clientPassword);
  client.subscribe("broker/counter");
  delay(100);

}

void loop() {
  for (int i = 0; i < message.length(); i++){
    Serial.print(message[i]);
  }
    client.loop();   
    delay(1000);
}
