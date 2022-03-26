#include <DHT.h>

#define BLYNK_TEMPLATE_ID "TMPLb1l1Ajhu"
#define BLYNK_DEVICE_NAME "ESP8266"
#define DHTPIN D2
#define DHTTYPE DHT11
#define relay 14
#define BLYNK_PRINT Serial
#define ECHOPIN 12
#define TRIGGERPIN 5

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 13 //กำหนดขาที่จะเชื่อมต่อ Sensor

DHT dht(DHTPIN, DHTTYPE);
char auth[] = "1j86a2a_2syMq5IEqwzorMs6GY-h8Mf0"; //ใส่ Token ของ Blynk
char wifiSSID[] = "妖精";
char wifiPASS[] = "Bravo_006";
int val = 0;
int map_val = 0 ;
int analogPin = A0;
long duration;
float distanceCM;
const long utcOffsetInSeconds = 25200;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", utcOffsetInSeconds);

void printTime();
//void distance(float velocity);

void setup()
{
  dht.begin();
  Serial.println("DHT11 Test!");
  Blynk.begin(auth,wifiSSID,wifiPASS);
  WiFi.begin(wifiSSID,wifiPASS);
  timeClient.begin();
  pinMode(relay,OUTPUT);
  Serial.begin(115200);
  int ledPin = D0;
  int ledPin3 = D1;
  int analogPin = A0;
  pinMode(analogPin,INPUT);
  pinMode(TRIGGERPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);
}
void loop()
{
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  float soundVelo = 0.034;
  if(isnan(t)||isnan(h)){
    Serial.println("Failed!");
  }
  else{
    printTime();
    Serial.print("Air Temperature :");
    Serial.print(t);
    Serial.println("*C");
    Blynk.virtualWrite(V0, t);
    Serial.print("Air Humidity : ");
    Serial.print(h);
    Serial.println("%");
    Blynk.virtualWrite(V1, h);
    val = analogRead(analogPin); 
    map_val = map(val, 0, 1024, 100, 0);
    Serial.print("Soil moisture = ");
    Serial.println(map_val);
    Blynk.virtualWrite(V2,map_val);
    Serial.println("Requesting temperatures...");
    sensors.requestTemperatures(); //อ่านข้อมูลจาก library
    Serial.print("Temperature is: ");
    Serial.print(sensors.getTempCByIndex(0)); // แสดงค่า อูณหภูมิ 
    Serial.println(" *C");
    Blynk.virtualWrite(V4,sensors.getTempCByIndex(0));
    digitalWrite(TRIGGERPIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGERPIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGERPIN, LOW);
    duration = pulseIn(ECHOPIN, HIGH);
    distanceCM = duration * soundVelo / 2.0;
    Serial.print("Distance : ");
    Serial.print(distanceCM);
    Blynk.virtualWrite(V5, distanceCM);
    Serial.println(" cm");
    delay(1000);
  }
  Blynk.run();
  delay(2000);
}
BLYNK_WRITE(3) //เป็นฟังก์ชันในการสั่งการทำงานเมื่อกดปุ่ม V3 ใน Blynk Application 
{
  if(param.asInt()){
    digitalWrite(relay, HIGH);
  }
  else{
    digitalWrite(relay, LOW);
  }
  delay(2000);
}

void printTime(){
    timeClient.update();
    Serial.print(timeClient.getHours());
    Serial.print(":");
    Serial.print(timeClient.getMinutes());
    Serial.print(":");
    Serial.println(timeClient.getSeconds());
}

/*void distance(float velocity){
    digitalWrite(TRIGGERPIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGERPIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGERPIN, LOW);
    duration = pulseIn(ECHOPIN, HIGH);
    distanceCM = duration * velocity / 2.0;
    Serial.print("Distance : ");
    Serial.print(distanceCM);
    Blynk.virtualWrite(V5, distanceCM);
    Serial.println(" cm");
}*/
