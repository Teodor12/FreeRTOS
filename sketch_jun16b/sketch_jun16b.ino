


//SETTING UP THE MC TO USE ONLY 1 CORE
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif


#define LEDPIN 2
#define PRESCALER 8
#define TIMER_COUNT 1000000

//GLOBAL DECLARATIONS
static hw_timer_t* timer_0_cfg = NULL;                     //hardware timer
static TickType_t task_delay = 2000 / portTICK_PERIOD_MS;  // 2 second delay
static volatile unsigned isr_counter;                      //count the number the isr was called
static portMUX_TYPE spinlock = portMUX_INITIALIZER_UNLOCKED;

void task_1(void* args) {
  while (1) {
    while (isr_counter > 0) {
      Serial.println(isr_counter);
      //forbiding interrupts while decrementing the value
      portENTER_CRITICAL_ISR(&spinlock);
      isr_counter--;
      portEXIT_CRITICAL(&spinlock);
    }
    vTaskDelay(task_delay);
  }
  
}

void IRAM_ATTR timer_0_ISR()  //this isr is stored in the internal RAM memory, in order to access it faster
{
  portENTER_CRITICAL_ISR(&spinlock);
  isr_counter++;
  portEXIT_CRITICAL(&spinlock);
}

void timer_0_init() {
  timer_0_cfg = timerBegin(0, 8, true);
  timerAttachInterrupt(timer_0_cfg, &timer_0_ISR, true);
  timerAlarmWrite(timer_0_cfg, 1000000, true);
  timerAlarmEnable(timer_0_cfg);
}

void setup() {
  Serial.begin(115200);
  xTaskCreatePinnedToCore(task_1, "task_1", 1024, NULL, 1, NULL, app_cpu);
  timer_0_init();
  vTaskDelete(NULL);
}
void loop() {
  // Do Nothing!
}