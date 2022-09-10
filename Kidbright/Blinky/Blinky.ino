void setup() {
 // Initailize LED pins
 pinMode(17, OUTPUT); // Bluetooth
 pinMode(2, OUTPUT); // Wifi
 pinMode(15, OUTPUT); // NTP
 pinMode(12, OUTPUT); // IoT
}
void loop() {
 // Turn on LEDs
 digitalWrite(17,HIGH);
 delay(100);
 digitalWrite(2,HIGH);
 delay(100);
 digitalWrite(15,HIGH);
 delay(100);
 digitalWrite(12,HIGH);
 delay(300);
 
 // Turn off LEDs
 digitalWrite(17,LOW);
 delay(100);
 digitalWrite(2,LOW);
 delay(100);
 digitalWrite(15,LOW);
 delay(100);
 digitalWrite(12,LOW);
 delay(300);
}
