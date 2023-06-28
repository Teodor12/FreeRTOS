#include <LiquidCrystal_I2C.h>

#include "LiquidCrystal_I2C.h"
#include "Wire.h"

#define BUTTON_PIN 2
#define LED_PIN 3
#define DEBOUNCE_DURATION 10


const int i2c_addr = 0x27;
LiquidCrystal_I2C lcd(i2c_addr, 16, 2);
byte last_button_state,current_button_state;
boolean led_state = LOW;



void init_lcd(){
      lcd.begin();
      lcd.clear();
      lcd.backlight();
}

void print_button_state(boolean state){
    lcd.setCursor(7, 0);
    if(state){
      lcd.print("ON");
    }else{
      lcd.print("OFF");
    }
}

void setup() {
    init_lcd();
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(9600);
    current_button_state = digitalRead(BUTTON_PIN);//this variable represents the current value of DIGITAL PIN 2, which HIGH by default, thanks to the INPUT_PULLUP configuration
}

void loop() {
    last_button_state = current_button_state; //saving the last state of PIN2 in each iteration
    current_button_state = digitalRead(BUTTON_PIN);
    if(last_button_state == 1 && current_button_state == 0){//if the button was pressed turn the led on/off
        led_state = !led_state;
        lcd.clear();
    }

    print_button_state(led_state); 
    digitalWrite(LED_PIN, led_state);
    
    delay(1);
}
