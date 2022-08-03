#if CONFIG_FREERTOS_UNICODE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

int usr_input = 0;
static const int ledPin = LED_BUILTIN;

void inputPeriod(void *p){
  while(1){
    Serial.print("Enter blinking period in ms >> \n");
    while(Serial.available() == 0){}
    usr_input = Serial.parseInt();
  }
}

void blinkLED(void *p){
  while(1){
      digitalWrite(ledPin,HIGH);
      vTaskDelay(usr_input/portTICK_PERIOD_MS);
      digitalWrite(ledPin,LOW);
      vTaskDelay(usr_input/portTICK_PERIOD_MS);
  }
}

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.println();
  Serial.println("----FreeRTOS Task Demo Challenge II----");

  xTaskCreatePinnedToCore(inputPeriod, "Input Period", 1024, NULL, 1, NULL, app_cpu);
  xTaskCreatePinnedToCore(blinkLED, "Blinking LED", 1024, NULL, 1, NULL, app_cpu);
}

void loop() {
  // put your main code here, to run repeatedly:

}
