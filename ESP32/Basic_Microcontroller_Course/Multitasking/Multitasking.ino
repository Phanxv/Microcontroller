#define G_PIN 22
#define Y_PIN 23

unsigned long int G_time = 0;
unsigned long int Y_time = 0;

int G_state = 0;
int Y_state = 0;

void setup()
{
  pinMode(G_PIN, OUTPUT);
  pinMode(Y_PIN, OUTPUT);
  Serial.begin(9600);
  digitalWrite(G_PIN, G_state);
  digitalWrite(Y_PIN,Y_state);
  G_time = millis();
  Y_time = millis();
}

void loop()
{
  if(millis() - G_time == 1000){
    G_state = 1 - G_state;
    digitalWrite(G_PIN, G_state);
    Serial.print("GREEN LED STATE : ");
    Serial.println(G_state);
    G_time = millis();
  }
  if(millis() - Y_time == 500){
    Y_state = 1 - Y_state;
    digitalWrite(Y_PIN, Y_state);
    Serial.print("YELLOW LED STATE : ");
    Serial.println(Y_state);
     Y_time = millis();
  }
}
