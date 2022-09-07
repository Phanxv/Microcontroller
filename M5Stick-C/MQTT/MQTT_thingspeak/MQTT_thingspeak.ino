/*
MQTT Client for Thinkspeak
Author : R. Sutthaweekul
*/

#include <M5StickCPlus.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_ADS1X15.h>

#include "config.h"

//WIFI
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWD;
const char* mqtt_server = MQTT_SERVER;

//MQTT client
WiFiClient espClient;
PubSubClient client(espClient);
//ADS

Adafruit_ADS1115 ads;

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
    //String clientId = "MQTT_Werkplaats";
    // Attempt to connect
    if (client.connect(SECRET_MQTT_CLIENT_ID, SECRET_MQTT_USERNAME, SECRET_MQTT_PASSWORD)) {
      Serial.println("connected");
      
      char subscribe_topic[100];
      sprintf(subscribe_topic,"channels/%s/subscribe",MQTT_CHANNEL);
      client.subscribe(subscribe_topic);
      sprintf(subscribe_topic,"channels/%s/subscribe/fields/%s",MQTT_CHANNEL,"field1");
      client.subscribe(subscribe_topic);
      
      Serial.println("subscribed");
      M5.Lcd.fillScreen(GREEN);
      M5.Lcd.setCursor(15, 10);
      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.setTextSize(4);
      M5.Lcd.printf("READY");
      //client.subscribe("tkkrlab/spacestate");

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
  client.setBufferSize(2048);

  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }
}

void loop() {
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  static int c=0;
  int16_t adc0, adc1, adc2, adc3;
  float volts0, volts1, volts2, volts3,accX = 0.0F,accY = 0.0F, accZ = 0.0F;
  char topic[40];
  char payload[100];
  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);
  adc2 = ads.readADC_SingleEnded(2);
  adc3 = ads.readADC_SingleEnded(3);

  volts0 = ads.computeVolts(adc0);
  volts1 = ads.computeVolts(adc1);
  volts2 = ads.computeVolts(adc2);
  volts3 = ads.computeVolts(adc3);

  M5.IMU.getAccelData(&accX,&accY,&accZ);
  sprintf(topic,"channels/%s/publish", MQTT_CHANNEL);
  sprintf(payload,"field1=%.4f&field2=%.4f&field3=%.4f&field4=%.4f&field5=6.2f&status=MQTTPUBLISH",volts0,volts1,volts2,volts3,accZ);
  client.publish(topic, payload);
  Serial.println("Topic and Payload sent");   
  delay(500);
  /*if(digitalRead(BUTTON_A_PIN) == 0) {
    char topic[40];
    char payload[100];
    sprintf(topic,"channels/%s/publish", MQTT_CHANNEL);
    sprintf(payload,"field1=%d&field2=%d&field3=%d&field4=%.4f&status=MQTTPUBLISH",c,c%2,c%3,c%4);
    client.publish(topic, payload);
    Serial.println("Button pressed");   
    delay(500);

    M5.Lcd.setCursor(20, 53);
    M5.Lcd.setTextColor(WHITE,GREEN);
    M5.Lcd.setTextSize(2);
    M5.Lcd.printf("c=%2d",c);
  
    c>=12? c=0: c++; 
  };*/
}
