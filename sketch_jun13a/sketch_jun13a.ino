#include <Arduino.h>
#include <WiFi.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHT_SENSOR_PIN  21 
#define DHT_SENSOR_TYPE DHT11

hw_timer_t * timer = NULL;
volatile unsigned int elapsed_seconds;
DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);
bool led_state = false;



void IRAM_ATTR timer_interrupt(){
  digitalWrite(2, !digitalRead(2));
  
  
}

void init_timer(){//timer to count overflow every 1 second
  timer = timerBegin(0, 80, true);//using TIMER0 for counting
  timerAttachInterrupt(timer, &timer_interrupt, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer); // Enable the alarm
}

void setup() {
  pinMode(21,INPUT);
  pinMode(2, OUTPUT);
  Serial.begin(921600);
  dht_sensor.begin();
  init_timer();
}

void loop() {
    float humidity = dht_sensor.readHumidity();
    float temperature = dht_sensor.readTemperature();
    Serial.print(humidity);
    Serial.print(" ");
    Serial.println(temperature);
}

