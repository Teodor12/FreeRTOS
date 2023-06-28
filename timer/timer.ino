/*The OCR1A/B (output compare reg.) registers are 16 bit registers, always compared to the TCNT1 register. 
TCCR1A/B are the timer/counter control registers. */

#define OUTPUT_PIN 2
bool led_state = false;

void timer_1_normal_mode() {
  //Setting timer mode to normal, via changing TCCR1A and TCCR1B registers bits to 0
  TCCR1A = 0;
  TCCR1B = 0;
}

void timer_1_prescaler_256() {
  //Setting the prescaler to 256
  TCCR1B |= (1 << CS12);
}

void enable_output_comp_A() {
  //Enabling output compare match mode between TCNT1 and OCR1A register
  TIMSK1 |= (1 << OCIE1A);
}

void enable_ctc() {  //enabling clear timer on compare output match mode
  TCCR1B |= (1 << WGM12);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  cli();

  timer_1_normal_mode();
  timer_1_prescaler_256();
  enable_output_comp_A();
  enable_ctc();
  Serial.begin(9600);
  OCR1A = 62500;
  sei();
}

ISR(TIMER1_COMPA_vect) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));  //toogling the voltage level of digital pin 2
}


void loop() {
}
