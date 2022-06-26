//only use 1 cpu for demo
#if CONFIG_FREERTOS_UNICODE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

const char msg[] = "The fleeting sense of love within this god forsaken hall";

//Task Handle
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

void startTask1(void *parameter){
  int msg_len = strlen(msg);
  while(1){
    Serial.println();
    for(int i = 0; i < msg_len; i++){
      Serial.print(msg[i]);
    }
    Serial.println();
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
}

void startTask2(void *parameter){
  while(1){
    Serial.print("*");
    vTaskDelay(100/portTICK_PERIOD_MS);
  }
}
void setup() {
  Serial.begin(300);
  Serial.println();
  Serial.println("----FreeRTOS Task Demo----");

  //print self priority
  Serial.print("Setup and loop task running on core ");
  Serial.println(xPortGetCoreID());
  Serial.print("With priority ");
  Serial.println(uxTaskPriorityGet(NULL));

  //Task to run
  xTaskCreatePinnedToCore(startTask1,
                          "Task 1",
                          1024,
                          NULL,
                          1,
                          &task_1,
                          app_cpu);
  //run once with higher priority
  xTaskCreatePinnedToCore(startTask2,
                          "Task 2",
                          1024,
                          NULL,
                          2,
                          &task_2,
                          app_cpu);
                         
}

void loop() {
  for(int i = 0; i < 3; i++){
    vTaskSuspend(task_2);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    vTaskResume(task_2);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
  //delete the lower priority
  if(task_1 != NULL){
    vTaskDelete(task_1);
    task_1 = NULL;
  }
}
