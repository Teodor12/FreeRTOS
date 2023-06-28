

//Definitions and constants
#define BUTTON_PIN 3
#define DEBOUNCE_TIME 150 //150 milliseconds button debounce



unsigned long last_millis = 0;
bool led_state = false;




//Function to prevent the bounce of the pushbutton
void debounce_interrupt(){
  if((millis() - last_millis) >= DEBOUNCE_TIME){
      led_state = !led_state;
      last_millis = millis();
  }
}

//Function to set up pins
void set_pins(){
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}


void setup() {
  set_pins();
  //Attaching the debounce_interrupt function to PIN 2.
  //The interrupt mode is set to FALLING though to the INPUT PULLUP mode on digital pin 3
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), debounce_interrupt, FALLING);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(LED_BUILTIN, led_state);
}
