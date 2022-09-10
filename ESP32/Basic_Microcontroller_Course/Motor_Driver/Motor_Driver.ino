#define IN1 25
#define IN2 26
#define ENA 27
#define CHANNEL 0
#define FREQUENCY 100
#define RESOLUTION 12
#define MAX_SPEED 4095

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  ledcSetup(CHANNEL, FREQUENCY, RESOLUTION);
  ledcAttachPin(ENA, CHANNEL);
}

void loop() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  ledcWrite(CHANNEL, 0);
  delay(5000);
  ledcWrite(CHANNEL, MAX_SPEED * 25 / 100);
  delay(5000);
  ledcWrite(CHANNEL, MAX_SPEED * 50 / 100);
  delay(5000);
  ledcWrite(CHANNEL, MAX_SPEED * 75 / 100);
  delay(5000);
  ledcWrite(CHANNEL, MAX_SPEED);
  delay(5000);
}
