
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

static const int led_pin = 2;
static const char* msg = "ESP-32";

static TaskHandle_t task_2_handle = NULL;
static TaskHandle_t task_3_handle = NULL;


//TASK - 1
void task_1(void* parameter) {
  while (1) {
    digitalWrite(led_pin, 1);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(led_pin, 0);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

//TASK - 2
void task_2(void* parameter) {
  while (1) {
    for (int i = 0; i < strlen(msg); i++) {
      Serial.print(msg[i]);
    }
    Serial.println();
    vTaskDelay(1000 / portTICK_PERIOD_MS);  //delaying the task, but other tasks can run while waiting
  }
}

void task_3(void* parameter) {
  while (1) {
    Serial.print("*");
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void setup() {

  Serial.println("Simultaneous tasks in RTOS:");
  Serial.begin(300);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  xTaskCreatePinnedToCore(task_2, "task_2", 1024, NULL, 1, &task_2_handle, app_cpu);
  xTaskCreatePinnedToCore(task_3, "task_3", 1024, NULL, 2, &task_3_handle, app_cpu);
  
}

void loop() {
  for(int i  = 0; i < 3; i++){
    vTaskSuspend(task_3_handle);
    vTaskDelay(2000/portTICK_PERIOD_MS);
    vTaskResume(task_3_handle);
    vTaskDelay(2000/portTICK_PERIOD_MS);
  }

  if(task_2_handle != NULL){
    vTaskDelete(task_2_handle);
    task_2_handle = NULL;
  }
}
