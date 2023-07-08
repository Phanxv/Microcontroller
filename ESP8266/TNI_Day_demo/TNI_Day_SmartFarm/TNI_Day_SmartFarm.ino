/* Comment this out to disable prints and save space */
//#define BLYNK_PRINT Serial

/* Fill-in your Template ID (only if using Blynk.Cloud) */
#define BLYNK_TEMPLATE_ID "TMPLkaPB9kat"
#define BLYNK_DEVICE_NAME "TNI DAY IOT SMART FARM"
#define BLYNK_AUTH_TOKEN "ykor-vWyGk0ELpotnS6OB1xPiss0hWFV"

/*#define IFTTT_Key "1NZh5X1acQnb-2B9-2YZz"
#define IFTTT_Event "Workshop16"*/

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
const char auth[] = BLYNK_AUTH_TOKEN;
const char ssid[] = "furue";//"TNI_IOT_WATER";//"dlink_DWR-920_A535";  // "Huawei_Y7";  // "TNI_WiFi_Officer";
const char pass[] = "Delta_006";//"zbSUM36557";  // "abcdefabcd";

#define WIFI_SSID "furue"//"dlink_DWR-920_A535"  
#define WIFI_PASS "Delta_006"//"zbSUM36557"

WiFiClient client;  
BlynkTimer timer;
void timerEvent();

/*---------------------------------------------------------*/
#include <Wire.h>
//#include <OneWire.h>
#include <LiquidCrystal_I2C.h>

//OneWire  ds(2);  // D4 on pin 2 (a 4.7K resistor is necessary)

#define SCL   D1
#define SDA   D2
#define Relay D3
#define LEDG  D5
#define US_T  D6 // LEDY
#define US_E  D7
#define Buzz  D8
#define DHTPIN D4
#define DHTTYPE DHT11

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define SOUND_VELOCITY 0.034
long duration, durationRead;
float distanceCm, distancePrv;
int i, analogPin = A0, val = 0, map_val = 0, count = 0;
int16_t raw;
float celsius;
float WS;
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  pinMode(Relay, OUTPUT);
  digitalWrite(Relay, LOW);

  pinMode(US_T, OUTPUT);
  pinMode(US_E, INPUT);

  digitalWrite(US_T, HIGH);
  delayMicroseconds(10);
  digitalWrite(US_T, LOW);
  duration = pulseIn(US_E, HIGH);

  Serial.begin(115200);
  //Wire.begin(SDA, SCL);
  dht.begin();
  // initialize the LCD
  lcd.begin();
  lcd.home();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.setCursor(5, 0);
  lcd.print("Hello");
  lcd.setCursor(4, 1);
  lcd.print("TNI IOT");

  pinMode(Buzz, OUTPUT);
  pinMode(LEDG, OUTPUT);

  digitalWrite(Buzz, HIGH);
  digitalWrite(LEDG, HIGH);
  delay(1000);
  digitalWrite(Buzz, LOW);
  digitalWrite(LEDG, LOW);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP()); 
  
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, timerEvent);
  lcd.clear();
}

void loop()
{
  Blynk.run();
  timer.run();
}

/*BLYNK_WRITE(V7)
{
  if (param.asInt()) digitalWrite(LEDG, HIGH);
  else               digitalWrite(LEDG, LOW);
}*/

BLYNK_WRITE(8) {
  if (param.asInt()){
    digitalWrite(Relay, HIGH);
    delay(1000);
  }
  else               digitalWrite(Relay, LOW);
}

void timerEvent()
{
  //lcd.clear();

  /*-- Soil Moisture ------------------------------------*/

  val = analogRead(analogPin);
  //if (val >= 300 && val <= 1000) map_val = map(val, 300, 1000, 99, 1);
  //else                          map_val = 0;

  if (val >= 0 && val <= 1023) map_val = map(val, 0, 1023, 99, 1);
  //else                          map_val = 0;

  Blynk.virtualWrite(V4, map_val);

  lcd.setCursor(9, 0);
  lcd.print("SM.    ");
  lcd.setCursor(12, 0);
  lcd.print(map_val);
  lcd.print("%");

  if(map_val < 50 && map_val > 0)
  {
    Blynk.virtualWrite(V8, 1);
    digitalWrite(Relay, HIGH);
  }
  else
  {
    Blynk.virtualWrite(V8, 0);
    digitalWrite(Relay, LOW);
  }

  /*-- Water Stock (Ultrasonic Sensor)--------------------*/

  for (i = 0; i < 3; i += 1)
  {
    digitalWrite(US_T, HIGH);
    delayMicroseconds(10);
    digitalWrite(US_T, LOW);
    durationRead = pulseIn(US_E, HIGH);

    if (duration > durationRead)
    {
      duration -= (duration - durationRead) / 2;
    }
    else if (duration < durationRead)
    {
      duration += (durationRead - duration) / 2;
    }
    delayMicroseconds(500);
  }

  distanceCm = duration * SOUND_VELOCITY / 2;

  if (distanceCm >= 28) distanceCm = distancePrv;
  else                distancePrv = distanceCm;
  WS = (27.5 - distanceCm) / 25 * 100;
  Blynk.virtualWrite(V5, WS);
  lcd.setCursor(9, 1);
  lcd.print("       ");
  lcd.setCursor(9, 1);
  lcd.print(WS);
  lcd.print("%");

  /*-- Temperature ----------------------------------------*/

  /*byte addr[] = {0x28, 0x3E, 0x38, 0x75, 0xD0, 0x01, 0x3C, 0xB3};
  byte data[12];

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  delay(1000);              // maybe 750ms is enough, maybe not
  ds.reset();
  ds.select(addr);
  ds.write(0xBE);           // Read Scratchpad
  for (i = 0; i < 9; i++)  // we need 9 bytes
  {
    data[i] = ds.read();
  }

  raw = (data[1] << 8) | data[0];*/
  celsius = dht.readTemperature();

  Blynk.virtualWrite(V6, celsius);
  lcd.setCursor(1, 0);
  lcd.print(celsius);
  lcd.setCursor(0, 1);
  lcd.print("celsius");

  /*if(count++ > 10)
  {  
    count = 0;
    delay(1000);
    send_webhook();
  }*/
}

/*void send_webhook()
{// function Send Webook to IFTTT
  // construct the JSON payload
  String jsonString = "";
  jsonString += "{\"value1\":\"";
  jsonString += celsius;
  jsonString += "\",\"value2\":\"";
  jsonString += map_val;
  jsonString += "\", \"value3\":\"";
  jsonString += (27.5 - distanceCm) / 25 * 100;
  jsonString += "\"}";
  
  int jsonLength = jsonString.length();  
  String lenString = String(jsonLength);
  // connect to the Maker event server
  client.connect("maker.ifttt.com", 80);
  // construct the POST request
  String postString = "";
  postString += "POST /trigger/";
  postString += IFTTT_Event;
  postString += "/with/key/";
  postString += IFTTT_Key;
  postString += " HTTP/1.1\r\n";
  postString += "Host: maker.ifttt.com\r\n";
  postString += "Content-Type: application/json\r\n";
  postString += "Content-Length: ";
  postString += lenString + "\r\n";
  postString += "\r\n";
  postString += jsonString; // combine post request and JSON
  client.print(postString);
  delay(500);
  client.stop();
}*/
