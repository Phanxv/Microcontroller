#include "BluetoothSerial.h"
#define IN1 26
#define IN2 25
#define IN3 33
#define IN4 32

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled!
#endif

void moveForward();
void moveBackward();
void turnLeft();
void turnRight();
void stopMove();

BluetoothSerial BTObject;
String message = "";

void setup() {
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  BTObject.begin("Robot");
}

void loop() {
  if(BTObject.available()){
    message = BTObject.readString();
    message.remove(message.length() - 2);
  }
  if(message == "W"){
    moveForward();
  }
  if(message == "S"){
    moveBackward();
  }
  if(message == "A"){
    turnLeft();
  }
  if(message == "D"){
    turnRight();
  }
  if(message == "HALT"){
    stopMove();
  }
}

void moveBackward(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveForward(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnRight(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);
}

void turnLeft(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMove(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);
}
