void Task(void *);

void setup() {
  Serial.begin(115200);
  xTaskCreatePinnedToCore
  (
    Task,
    "TaskCore0",
    10000,
    NULL,
    1,
    NULL,
    0
  );
}

void loop() {
  Serial.print("loop() is running on core : ");
  Serial.println(xPortGetCoreID());
  delay(1000);
}

void Task(void *){
  while(true){
    Serial.print("Task is running on core : ");
    Serial.println(xPortGetCoreID());
    delay(1000);
  }
}
