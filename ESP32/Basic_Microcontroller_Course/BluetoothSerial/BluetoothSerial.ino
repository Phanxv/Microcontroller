#include "BluetoothSerial.h"

BluetoothSerial BTObject;
String message = "";

void setup() {
  Serial.begin(115200);
  BTObject.begin("ESP32test_TEEN");
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("The device is ready");
}

void loop() {
  if(BTObject.available()){
    message = BTObject.readString();
    message.remove(message.length() - 2);
    Serial.println(message);
  }
  if(message == "ON"){
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else if(message == "OFF"){
    digitalWrite(LED_BUILTIN, LOW);
  }
}
