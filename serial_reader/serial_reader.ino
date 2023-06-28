#include "SoftwareSerial.h"

SoftwareSerial mySerial(2,3);//custom Serial ports, RX and finally TX

void setup() {
    Serial.begin(9600);
    mySerial.begin(9600);
    delay(5000);

}

void loop() {
    String input_string="";
    boolean string_ready = false;
    while (mySerial.available()) {
        input_string = mySerial.readString();
        string_ready = true;
    }
    if(string_ready){
        Serial.println("String received: " + input_string);
    }

}
