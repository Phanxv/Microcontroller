//only use 1 cpu for demo
#if CONFIG_FREERTOS_UNICODE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

//Pin init
static const int led_pin = LED_BUILTIN;

//Task Blinking LED
void blinkLED(void *parameter){
  while(1){
    digitalWrite(led_pin, HIGH);
    vTaskDelay(500/portTICK_PERIOD_MS); //vTaskDelay expected number of tick as an argument //delay 500 tick divided by tick per millisec
    digitalWrite(led_pin, LOW);
    vTaskDelay(500/portTICK_PERIOD_MS);
  }
}
void setup() {
  //config pin
  pinMode(led_pin, OUTPUT);

  //pinned task to cpu core
  xTaskCreatePinnedToCore(      //Use xTaskCreate() in vanilla FreeRTOS
                 blinkLED,      //Task to pin
                 "Blink LED",   //Name of task
                 1024,          //Stack size (bytes in ESP32 word in vanilla FreeRTOS) //minimum 768 for empty task
                 NULL,          //Parameter to pass to function
                 1,             //Task priority
                 NULL,          //Task handle
                 app_cpu);      //Core to run task
}

void loop() {
  // put your main code here, to run repeatedly:

}
