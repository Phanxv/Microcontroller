/*
MQTT Client for Thinkspeak
Author : R. Sutthaweekul
*/

#include <M5StickCPlus.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_ADS1X15.h>
#include <ArduinoJson.h>
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


float gyroX = 0.0F, gyroY = 0.0F, gyroZ = 0.0F, accX = 0.0F, accY = 0.0F, accZ = 0.0F, pitch = 0.0F, roll = 0.0F, yaw = 0.0F;


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
      
      //char subscribe_topic[100];
      //sprintf(subscribe_topic,"channels/%s/subscribe",MQTT_CHANNEL);
      client.subscribe("topgun/Jezuz/arm/movement");
      //sprintf(subscribe_topic,"channels/%s/subscribe/fields/%s",MQTT_CHANNEL,"field1");
      client.subscribe("topgun/Jezuz/fingers/grasp");
      
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
   M5.Imu.Init();
  M5.Lcd.fillScreen(BLUE);
  M5.Lcd.setCursor(15, 10);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(5);
  M5.Lcd.printf("BOOT");
    
  Serial.begin(115200);
  
  setup_wifi();
  client.setServer(mqtt_server, 15958);
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
  int adc0, adc1, adc2, adc3;
  char topic1[40], topic2[40];
  StaticJsonDocument<256> jsonData1;
  StaticJsonDocument<512> jsonData2;
  char payload1[100], payload2[200];
  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);
  adc2 = ads.readADC_SingleEnded(2);
  adc3 = ads.readADC_SingleEnded(3);

  M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ);
  M5.IMU.getAccelData(&accX, &accY, &accZ);
  M5.IMU.getAhrsData(&pitch, &roll, &yaw);
  sprintf(topic1,"topgun/Jezuz/fingers/grasp/");
  sprintf(topic2,"topgun/Jezuz/arm/move/");
  //sprintf(payload,"field1=%d&field2=%d&field3=%d&field4=%d&field5=%d&status=MQTTPUBLISH",adc0,adc1,adc2,adc3,accZ);
  jsonData1["finger1"] = adc0;
  jsonData1["finger2"] = adc1;
  jsonData1["finger3"] = adc2;
  jsonData1["finger4"] = adc3;
  jsonData2["gx"] = gyroX;
  jsonData2["gy"] = gyroY;
  jsonData2["gz"] = gyroZ;
  jsonData2["ax"] = accX;
  jsonData2["ay"] = accY;
  jsonData2["az"] = accZ;
  jsonData2["pitch"] = pitch;
  jsonData2["roll"] = roll;
  jsonData2["yaw"] = yaw;
  serializeJson(jsonData1, payload1);
  serializeJson(jsonData2, payload2);
  client.publish(topic1, payload1);
  Serial.println("Topic1 and Payload1 sent");   
  client.publish(topic2, payload2);
  Serial.println("Topic2 and Payload2 sent"); 
  delay(500);
}
