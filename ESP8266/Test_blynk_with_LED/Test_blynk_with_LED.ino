#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID " " //ใส่ TEMPLATE_ID ของ Blynk
#define BLYNK_DEVICE_NAME " " //ใส่ DEVICE_NAME ของ Blynk
#define LED D5
char auth[] = " "; //ใส่ Token ของ Blynk

void setup()
{
  dht.begin();
  Serial.println("DHT11 Test!");
  Blynk.begin(auth," WIFI_name " , " Password_wifi");
  pinMode(LED,OUTPUT);
  Serial.begin(115200);
}
void loop()
{
  Blynk.run();
}
BLYNK_WRITE(3) //เป็นฟังก์ชันในการสั่งการทำงานเมื่อกดปุ่ม V3 ใน Blynk Application 
{
  if(param.asInt()){
    digitalWrite(LED, HIGH);
  }
  else{
    digitalWrite(LED, LOW);
  }
  delay(2000);
}
