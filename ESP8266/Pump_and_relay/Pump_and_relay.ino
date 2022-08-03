int relay1 = 15 ;
void setup() {
  pinMode(relay1, OUTPUT);      // Initialize the LED_BUILTIN pin as an output
}

// the loop function runs over and over again forever
void loop() {
 digitalWrite(relay1, 0);   
 delay(6000); 
 digitalWrite(relay1, 1);   // สั่งปิดรีเลย์
 delay(6000); 
}
