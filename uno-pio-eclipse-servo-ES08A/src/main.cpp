/**
 * servo
 *
 * https://www.arduino.cc/en/Tutorial/Knob
 */

#include <Arduino.h>
#include <Servo.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

Servo myservo;  // create servo object to control a servo

constexpr int POT_PIN = 0; // analog pin used to connect the potentiometer
constexpr int SERVO_PIN = 9;

template<typename A, typename B, typename C, typename D, typename E>
constexpr void SERIAL_PRINT_5(const A a, const B& b, const C& c, const D& d, const E& e) {
	Serial.print(a);
	Serial.print(b);
	Serial.print(c);
	Serial.print(d);
	Serial.print(e);
	Serial.println();
}

void setup() {
	Serial.begin(9600);

	pinMode(LED_BUILTIN, OUTPUT);

    myservo.attach(SERVO_PIN);
}

void blink_led() {
    // turn the LED on (HIGH is the voltage level)
    digitalWrite(LED_BUILTIN, HIGH);

    // wait for a second
    delay(500);

    // turn the LED off by making the voltage LOW
    digitalWrite(LED_BUILTIN, LOW);

    // wait for a second
    delay(500);
}

void loop() {
    int pot_val = analogRead(POT_PIN); // reads the value of the potentiometer (value between 0 and 1023)
    int servo_angle = map(pot_val, 0, 1023, 0, 180); // scale it to use it with the servo (value between 0 and 180)

    SERIAL_PRINT_5("pot: ", pot_val, " => ", "angle: ", servo_angle);

    myservo.write(servo_angle); // sets the servo position according to the scaled value

    delay(200); // waits for the servo to get there

    blink_led();
}
