#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID " " //ใส่ TEMPLATE_ID ของ Blynk
#define BLYNK_DEVICE_NAME " " //ใส่ DEVICE_NAME ของ Blynk
#define Relay D2

int ledPin = D5;
int ledPin3 = D6;
int analogPin = A0; //ประกาศตัวแปร ให้ analogPin แทนขา analog ขาที่5
int val = 0;
int map_val = 0 ;
char auth[] = " "; //ใส่ Token ของ Blynk

void setup()
{
  Serial.println("DHT11 Test!");
  Blynk.begin(auth," WIFI_name " , " Password_wifi");
  pinMode(ledPin, OUTPUT);  // sets the pin as output
  pinMode(ledPin3, OUTPUT);  // sets the pin as output
  pinMode(Relay,OUTPUT);
  Serial.begin(115200);
}
void loop()
{
  val = analogRead(analogPin);  //อ่านค่าสัญญาณ analog ขา5 ที่ต่อกับ Soil Moisture Sensor Module v1
  map_val = map(val, 0, 1024, 100, 0); //ปรับเปลี่ยนค่าจาก 0-1024 เป็น 0-100
  Serial.print("val = "); // พิมพ์ข้อมความส่งเข้าคอมพิวเตอร์ "val = "
  Serial.println(map_val); // พิมพ์ค่าของตัวแปร val
  Blynk.virtualWrite(V0,map_val);
  if (map_val > 50) { 
    digitalWrite(ledPin, LOW); // สั่งให้ LED ที่ Pin2 ดับ
    digitalWrite(ledPin3, HIGH); // สั่งให้ LED ที่ Pin3 ติดสว่าง
  }
  else {
    digitalWrite(ledPin, HIGH); // สั่งให้ LED ที่ Pin2 ติดสว่าง
    digitalWrite(ledPin3, LOW); // สั่งให้ LED ที่ Pin3 ดับ
  }
  Blynk.run();
  delay(2000);
}
BLYNK_WRITE(3) //เป็นฟังก์ชันในการสั่งการทำงานเมื่อกดปุ่ม V3 ใน Blynk Application 
{
  if(param.asInt()){
    digitalWrite(Relay, HIGH);
  }
  else{
    digitalWrite(Relay, LOW);
  }
  delay(2000);
}
