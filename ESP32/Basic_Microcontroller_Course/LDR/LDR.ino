int LDR_pin = 36;
int LDR_val = 0;
int LED_pin = 22;

void setup() {
  pinMode(LDR_pin, INPUT);
  pinMode(LED_pin, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  LDR_val = analogRead(LDR_pin);
  Serial.print("LRD value : ");
  Serial.println(LDR_val);
  if(LDR_val <20){
    digitalWrite(LED_pin, HIGH);
  }
  else{
    digitalWrite(LED_pin, LOW);
  }
}
