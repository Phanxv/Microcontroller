#define ADC_PIN 36

int digital_val = 0;
float voltage_val = 0;

void setup(){
  pinMode(ADC_PIN, INPUT);
  Serial.begin(115200);
}

void loop(){
  digital_val = analogRead(ADC_PIN);
  voltage_val = digital_val / 4095.0 * 3.3;
  Serial.print("digital value : ");
  Serial.println(digital_val);
  Serial.print("voltage value : ");
  Serial.println(voltage_val);
  delay(500);
}
