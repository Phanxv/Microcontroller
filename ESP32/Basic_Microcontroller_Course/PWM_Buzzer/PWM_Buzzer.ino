//โน้ต
#define A 440
#define B 494
#define C 523
#define D 587
#define E 659
#define F 698
#define G 784
//PWM
#define CHANNEL 0
#define PWM_PIN 22
#define RESOLUTION 12

void play(int frequency,int play_time){
  ledcSetup(CHANNEL, frequency, RESOLUTION);
  ledcAttachPin(PWM_PIN, CHANNEL);
  ledcWrite(CHANNEL, 500);
  delay(play_time);
  ledcWrite(CHANNEL, 0);
}

void setup() {
  
}

void loop() {
  play(A,500);
  play(B,500);
  play(C,500);
  play(D,500);
  play(E,500);
  play(F,500);
  play(G,500);
}
