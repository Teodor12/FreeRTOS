

// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Globals
static int shared_var = 0;
static SemaphoreHandle_t mutex;


// Increment shared variable (the wrong way)
void incTask(void *parameters) {

  int local_var;

  while (1) {
    if (xSemaphoreTake(mutex, 0) == pdTRUE) {  //increment the global variable if the mutex(key) is available
      local_var = shared_var;
      local_var++;
      vTaskDelay(random(100, 500) / portTICK_PERIOD_MS);
      shared_var = local_var;
      Serial.println(shared_var);
      xSemaphoreGive(mutex);  // giving back the mutex(key)
    } else {
      //DO NOTHING!
    }
  }
}


void setup() {

  // Hack to kinda get randomness
  randomSeed(analogRead(0));

  // Configure Serial
  Serial.begin(115200);

  // Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Race Condition Demo---");

  mutex = xSemaphoreCreateMutex();  // Creating the mutex for tasks


  xTaskCreatePinnedToCore(incTask, "Increment Task 1", 1024, NULL, 1, NULL, app_cpu);

  // Start task 2
  xTaskCreatePinnedToCore(incTask, "Increment Task 2", 1024, NULL, 1, NULL, app_cpu);

  // Delete "setup and loop" task
  vTaskDelete(NULL);
}

void loop() {
  // Execution should never get here
}
