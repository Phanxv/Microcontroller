#define BUTTON 22

void showPush();

void setup() {
  pinMode(BUTTON, INPUT);
  attachInterrupt(BUTTON, showPush, RISING);
  Serial.begin(115200);
  Serial.println("Setup started");
}

void loop() {
  Serial.println("This is main program");
  delay(5000);
}

void showPush(){
  Serial.println("Interrupt occurs");
}
