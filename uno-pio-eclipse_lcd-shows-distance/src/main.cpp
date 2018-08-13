#include "Arduino.h"
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

// Ultrasonic Sensor HC-SR04
const int trigPin = 9;
const int echoPin = 10;

// LCD
#define I2C_ADDRESS 0x27
const int WIDTH = 20;
const int HEIGHT = 4;
LiquidCrystal_PCF8574 lcd(I2C_ADDRESS);

void setup() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    lcd.begin(WIDTH, HEIGHT); // initialize the lcd
}

int read_distance_in_cm() {
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    // Sets the trigPin on HIGH state for 10 microseconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Reads the echoPin, returns the sound wave travel time in microseconds
    long duration = pulseIn(echoPin, HIGH);

    // Calculating the distance in cm
    int distance = duration * 0.034 / 2;

    return distance;
}

void lcd_show_lines(const String& s0, const String& s1) {
    lcd.setBacklight(255);
    lcd.home();
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print(s0);

    lcd.setCursor(0, 1);
    lcd.print(s1);
}

void loop() {
    int dist = read_distance_in_cm();

    lcd_show_lines("Distance:", String(dist) + "cm");

    delay(1000);
}

