#include <FastLED.h>

#define DATA_PIN 3
#define POT_PIN A6
#define LED_NUM 60
#define DEBOUNCE_TIME 150  //a usual pushbutton prell lasts 150 ms

//Aliasing the digital pins to custom names
#define BUTTON_0 8
#define BUTTON_1 9
#define BUTTON_2 10
#define BUTTON_3 11

//Aliasing the led strip effects to custom names
#define BLACK \
  CRGB { \
    0, 0, 0, \
  }
#define RED \
  CRGB { \
    255, 0, 0 \
  }
#define GREEN \
  CRGB { \
    0, 255, 0 \
  }
#define BLUE \
  CRGB { \
    0, 0, 255 \
  }


CRGB current_mode = NULL;
CRGB leds[LED_NUM];
uint8_t hue = 0;
uint8_t brightness = 0;


//PIN CHANGE INTERRUPT ON PCINT GROUP 0 TO DETERMINE WHICH BUTTON WAS PRESSED
ISR(PCINT0_vect) {
  static volatile unsigned long last_millis = 0;  //debouncing the button
  if ((millis() - last_millis) >= DEBOUNCE_TIME) {
    //current_mode = pin_number;
    if (digitalRead(BUTTON_0)) {
      //Serial.println("Button 0 was pressed!");
      current_mode = BLACK;
    } else if (digitalRead(BUTTON_1)) {
      //Serial.println("Button 1 was pressed");
      current_mode = RED;
    } else if (digitalRead(BUTTON_2)) {
      //Serial.println("Button 2 was pressed");
      current_mode = GREEN;
    } else if (digitalRead(BUTTON_3)) {
      //Serial.println("Button 3 was pressed");
      current_mode = BLUE;
    }
    last_millis = millis();  //save the last program time
  }
}

void set_color() {

  for (int i = 0; i < LED_NUM; i++) {
    leds[i] = current_mode;
  }
  FastLED.show();
}

void setup() {
  Serial.begin(9600);
  PCICR |= B00000001;   //activating the pin change interrupts for GROUP 0
  PCMSK0 |= B00001111;  //D8,D9,D10 and D11 pins can trigger an interrupt
  current_mode = BLACK; //setting the default lighting mode to black
  FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, LED_NUM);
  FastLED.setBrightness(50); //setting the default brightness for the led strip
}

void loop() {
  FastLED.setBrightness(map(analogRead(POT_PIN),0,1023,0,255));
  set_color();
}