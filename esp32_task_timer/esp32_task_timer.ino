#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

#define LEDPIN 2


static TaskHandle_t led_task_handle = NULL;
static TimerHandle_t timer_handle_3 = NULL;

void ISR(void* args) {  //this isr is attached to timer with id 1. Every 500 milisecond this isr is called.
  xTaskResumeFromISR(led_task_handle);//
}

void led_task(void* args) { //task to jump from the isr
  static bool led_state = false;
  while (1) {
    vTaskSuspend(NULL);//we add suspend to run only once
    led_state = !led_state;
    digitalWrite(LEDPIN, led_state);
    Serial.println("Task entered.");
  }
}

void setup() {
  pinMode(2, OUTPUT);
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("----FREERTOS Timer Demo----");

  xTaskCreate(led_task, "Task2", 1024, NULL, 1, &led_task_handle);
  timer_handle_3 = xTimerCreate("periodic timer", 1000 / portTICK_PERIOD_MS, pdTRUE, (void*)2, ISR);


  if (timer_handle_3 == NULL) {
    Serial.println("Error creating the timers");
  } else {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    Serial.println("Starting timers...");
    xTimerStart(timer_handle_3, portMAX_DELAY);
  }
  vTaskDelete(NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
}
