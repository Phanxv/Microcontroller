#include <Arduino.h>

int cnt1 = 0, cnt2 = 0;

void task1(void * param){
  while(1){
    Serial.print("task 1 counter : ");
    Serial.println(cnt1++);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void task2(void * param){
  while(1){
    Serial.print("task 2 counter : ");
    Serial.println(cnt2++);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  xTaskCreate(task1, 
              "Task1",
              1000,
              NULL,
              1,
              NULL);
  xTaskCreate(task2, 
              "Task2",
              1000,
              NULL,
              1,
              NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
}