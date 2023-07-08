const int trigger = 8, echo = 10;
const int red = 13, yellow = 12, green = 11;
float dist, trav_t;

void setup(){
  Serial.begin(9600);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);
}

void loop(){        
  dist = getDistance();
  Serial.print ("Distance = ");
  Serial.print (dist);         
  Serial.print (" cm");         
  Serial.write (10);
  digitalWrite(green, HIGH);
  digitalWrite(yellow, HIGH); 
  digitalWrite(red, HIGH);
  if(dist < 50){
    digitalWrite(green, LOW);
    digitalWrite(yellow, HIGH); 
    digitalWrite(red, HIGH);
    if(dist < 40){
      digitalWrite(green, LOW);
      digitalWrite(yellow, LOW); 
      digitalWrite(red, HIGH);
        if(dist < 30){
        digitalWrite(green, LOW);
        digitalWrite(yellow, LOW); 
        digitalWrite(red, LOW);
      }
    }
  }
  delay(100);               
}                             

float getDistance(){
  digitalWrite(trigger,LOW);
  delayMicroseconds(5);        
  digitalWrite(trigger,HIGH);  
  delayMicroseconds(10);      
  digitalWrite(trigger,LOW); 
  trav_t = pulseIn(echo,HIGH);      
  dist = trav_t*0.034 / 2;
  return dist;
}