#include <stdlib.h>

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

#define BUFFERSIZE 10

static const int led_pin = 2;
static unsigned led_delay = 500;
String input_buffer = "";


//TASK TO TOOGLE LED AT A GIVEN RATE
void toogle_led(void* parameter) {
  while (1) {
    digitalWrite(led_pin, 1);
    vTaskDelay(led_delay / portTICK_PERIOD_MS);
    digitalWrite(led_pin, 0);
    vTaskDelay(led_delay / portTICK_PERIOD_MS);
  }
}

//TAKS TO READ SERIAL CHANNEL
void read_serial(void* parameter) {
  while (1) {
    if (Serial.available() > 0) {
      int current_character = Serial.read();
      if (isDigit(current_character)) {
        input_buffer += (char)current_character;
      }
      if (current_character == '\n') {
        led_delay = input_buffer.toInt();
        Serial.println(led_delay);
        input_buffer = "";
      }
    }
  }
}

void read_serial_1(void* parameter) {
  char current_character;
  char buf[BUFFERSIZE];
  uint8_t idx = 0;
  memset(buf, 0, BUFFERSIZE);
  while (1) {
    if (Serial.available() > 0) {
      current_character = Serial.read();
      if (isDigit(current_character) && idx < BUFFERSIZE) {
        buf[idx++] = current_character;
      } 
      if (current_character == '\n') {
        led_delay = atoi(buf);
        Serial.println(led_delay);
        idx = 0;
        memset(buf, 0, BUFFERSIZE);
      }
    }
  }
}




void setup() {
  pinMode(led_pin, OUTPUT);
  Serial.begin(9600);
  while (!Serial) {}
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println("Multi-task LED Demo");
  Serial.println("Enter a number in milliseconds to change the LED delay.");

  xTaskCreatePinnedToCore(read_serial_1, "Read Serial", 1024, NULL, 1, NULL, app_cpu);
  xTaskCreatePinnedToCore(toogle_led, "Toogle led", 1024, NULL, 1, NULL, app_cpu);

  // Delete "setup and loop" task
  vTaskDelete(NULL);
}



void loop() {
  // Execution should never get here
}
