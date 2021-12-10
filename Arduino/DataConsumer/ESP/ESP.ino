#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//wifi
const char* ssid = "ssid1";
const char* password = "password1";
const char* mqtt_server = "192.168.4.1";

//variables globals del callback
/*
byte* buffer;
boolean Rflag=false;
int r_len;
*/

byte var;
WiFiClient espClient;
PubSubClient client(espClient);

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



String messageToPass[1];
String contentString;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  char content[length];
  for(int i = 0; i < length; i++){
    Serial.print((char) payload[i]);
    
    contentString.concat((char) payload[i]);
    content[i] = (char)payload[i];
    
    Serial.print("\n");
  }
  messageToPass[0] = contentString;
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
 
  client.subscribe("#");
  delay(100);

}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    Serial.print("Message: ");
    for(int i = 0; i < sizeof(messageToPass); i++){
      Serial.print(messageToPass[i]);
      
    }
    Serial.print("\n");
    //Serial.print("Message: " + content + "\n");
   
    client.loop();   


}
