int potpin = 36;
int outpin = 22;
int potVal, potRaw;
int freq = 5000;
int ledChannel = 0;
int resolution = 12;
void setup() {
  Serial.begin(115200);
  pinMode(potpin, INPUT);
  pinMode(outpin, OUTPUT);
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(outpin, ledChannel);
}

void loop() {
  potRaw = analogRead(potpin);
  potVal = map(potRaw, 0, 4095, 0, 100);
  Serial.println(potVal);
  Serial.println(potRaw);
  ledcWrite(ledChannel, potRaw);
  delay(1);
}
