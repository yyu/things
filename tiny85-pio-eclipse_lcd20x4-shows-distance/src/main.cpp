#include "Arduino.h"
#include <TinyWireM.h> // instead of Wire.h which won't work for ATtiny85
#include "LiquidCrystal_PCF8574_ATtiny85.h"

// Ultrasonic Sensor HC-SR04
const int trig_pin = 3;
const int echo_pin = 4;

// LCD
#define I2C_ADDRESS 0x27
const int WIDTH = 20;
const int HEIGHT = 4;

LiquidCrystal_PCF8574_ATtiny85 lcd(I2C_ADDRESS);

void setup() {
    pinMode(trig_pin, OUTPUT);
    pinMode(echo_pin, INPUT);

    lcd.begin(WIDTH, HEIGHT); // initialize the lcd
    lcd.setBacklight(255);
    lcd.home();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Distance:");
}

void get_and_display_distance_in_cm() {
    // Clears the trig_pin
    digitalWrite(trig_pin, LOW);
    delayMicroseconds(2);

    // Sets the trig_pin on HIGH state for 10 microseconds
    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_pin, LOW);

    // Reads the echo_pin, returns the sound wave travel time in microseconds
    long duration = pulseIn(echo_pin, HIGH);

    lcd.setCursor(0, 2);
    lcd.print("duration: " + String(duration));

    // Calculating the distance in cm
    int distance = duration * 0.034 / 2;

    lcd.setCursor(0, 3);
    lcd.print(String(distance) + " cm        ");
}

void loop() {
    get_and_display_distance_in_cm();
    delay(1000);
}
