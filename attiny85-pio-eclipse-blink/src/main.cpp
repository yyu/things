#include "Arduino.h"

/*
 * Tiny AVR Programmer:
 *     * https://www.sparkfun.com/products/11801
 *     * https://github.com/sparkfun/Tiny-AVR-Programmer
 *     * https://static.sparkfun.com/datasheets/Dev/AVR/Tiny_Programmer.pdf
 *
 * useful info:
 *     * https://learn.sparkfun.com/tutorials/tiny-avr-programmer-hookup-guide/
 *
 * I2C on attiny85
 *     * http://playground.arduino.cc/Code/USIi2c
 */

// Sparkfun Tiny AVR Programmer has an LED connected to PIN 0
#define LED_PIN 1 // so let's use PIN 1 for testing

void setup() {
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    digitalWrite(LED_PIN, HIGH); delay(500);
    digitalWrite(LED_PIN, LOW);  delay(500);
}
