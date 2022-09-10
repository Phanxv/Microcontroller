#define BLYNK_TEMPLATE_ID "TMPLuJ6PbDdS"
#define BLYNK_DEVICE_NAME "Test"
#define BLYNK_AUTH_TOKEN "-oxLHp9vMgf56Eji64R-rR9lU11yScb6"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"

char ssid[] = "TNI_C406WIFI";
char pass[] = "TNIC406WIFI";

#define DHTPIN 22
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V5, h); 
  Blynk.virtualWrite(V6, t); 
}

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  dht.begin();
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
}
