#define trigPin D1
#define echoPin D2

#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;
const int red = D5, yellow = D6, green = D7;
 
void setup() {
  Serial.begin(115200); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
}

void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_VELOCITY/2;
  
  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;
  
  // Prints the distance on the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);

  if(distanceCm > 30){
   digitalWrite(green, HIGH);
   digitalWrite(yellow, HIGH);
   digitalWrite(red, HIGH);
   Serial.print(">50");
  }
  if(distanceCm < 30 && distanceCm > 20){
   digitalWrite(green, LOW);
   digitalWrite(yellow, HIGH);
   digitalWrite(red, HIGH);
   Serial.print("<50");
  }
  if(distanceCm < 20 && distanceCm > 10){
   digitalWrite(green, LOW);
   digitalWrite(yellow, LOW);
   digitalWrite(red, HIGH);
   Serial.print("<40");
  }
  if(distanceCm < 10){
   digitalWrite(green, LOW);
   digitalWrite(yellow, LOW);
   digitalWrite(red, LOW);
   Serial.print("<30");
  }
  delay(100);
}
