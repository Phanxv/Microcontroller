#include "BluetoothSerial.h"
#include <ESP32Servo.h>
#define IN1 32
#define IN2 33
#define IN3 25
#define IN4 26

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled!
#endif

int servoPin = 12;
int relayPin = 15;
unsigned long time_now_servo = 0;
unsigned long time_now_relay = 0;

void moveForward();
void moveBackward();
void turnLeft();
void turnRight();
void stopMove();
void shooter();

BluetoothSerial BTObject;
Servo myservo;

String message = "";

void setup() {
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  myservo.write(0);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
  BTObject.begin("Robot");
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);
  myservo.attach(servoPin, 1000, 2000);
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
  if(message == "H"){
    stopMove();
  }
  if(message == "O"){
    shooter();
  }
}

void turnLeft(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void moveForward(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void moveBackward(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void stopMove(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);
}

void shooter(){
  time_now_servo = millis();
  digitalWrite(relayPin, LOW);
  while(millis() < time_now_servo + 5000){

  }
  time_now_relay = millis();
  myservo.write(180);
  while(millis() < time_now_relay + 500){
    
  }
  myservo.write(0);
  digitalWrite(relayPin, HIGH);
  message = "";
}
