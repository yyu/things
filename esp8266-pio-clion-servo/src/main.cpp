#include <Arduino.h>

#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int pos = 0;

void setup() {
    myservo.attach(14);  // attaches the servo on GIO2 to the servo object
}
void loop() {
    myservo.write(pos ? 100 : 10);
    pos = 1 - pos;
    delay(1000);
}
