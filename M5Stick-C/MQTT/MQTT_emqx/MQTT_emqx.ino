/*
MQTT Client for EMQX
Author : R. Sutthaweekul
*/

#include <M5StickC.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include "config.h"

//WIFI
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWD;
const char* mqtt_server = MQTT_SERVER;

//MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {

  delay(10);
  // connect to WiFi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");  
  }

  //randomSeed(micros());

  Serial.println();
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
}

void reconnect() {
  // Loop until it is connected
  while (!client.connected()) {

    Serial.print("Attempting MQTT connection...");
    //String clientId = "MQTT_1";
    // Attempt to connect
    if (client.connect(MQTT_CLIENT_ID)) {
      Serial.println("connected");
      
      char topic[40];
      sprintf(topic,"%s/#", MQTT_TOPIC_PREFIX);
      
      client.subscribe(topic);
      
      Serial.println("subscribed");
      M5.Lcd.fillScreen(GREEN);
      M5.Lcd.setCursor(15, 10);
      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.setTextSize(4);
      M5.Lcd.printf("READY");

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {

  M5.begin();
  M5.Lcd.setRotation(1);
  
  M5.Lcd.fillScreen(BLUE);
  M5.Lcd.setCursor(15, 10);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(5);
  M5.Lcd.printf("BOOT");
    
  Serial.begin(115200);
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  //client.setBufferSize(2048);

  pinMode(BUTTON_A_PIN, INPUT);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  static int c=0;
  
  if(digitalRead(BUTTON_A_PIN) == 0) {
    char topic[40];
    char payload[100];
    sprintf(topic,"%s/fingers", MQTT_TOPIC_PREFIX);
    sprintf(payload,"{\"finger1\"=%d,\"finger2\"=%d,\"finger3\"=%d,\"finger4=%d,\"finger5\"=%d}",c,c%2,c%3,c%4,c%5);
    client.publish(topic, payload);
    Serial.println("Button pressed");   
    delay(500);

    M5.Lcd.setCursor(20, 53);
    M5.Lcd.setTextColor(WHITE,GREEN);
    M5.Lcd.setTextSize(2);
    M5.Lcd.printf("c=%2d",c);
  
    c>=12? c=0: c++; 
  }
}
