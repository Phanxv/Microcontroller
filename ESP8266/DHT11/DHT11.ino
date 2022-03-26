#include <DHT.h>
#define DHTPIN D2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
void setup() {
  dht.begin();
  Serial.println("DHT11 Test!");
  Serial.begin(9600);
}

void loop() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if(isnan(t)||isnan(h)){
    Serial.println("Failed!");
  }
  else{
    Serial.print("Temp :");
    Serial.print(t);
    Serial.println("*C");
    Serial.print("Humid : ");
    Serial.print(h);
    Serial.println("%");
    Serial.println("-------------------------------------");
  }
  delay(2000);

}
