#include "Arduino.h"
#include <TinyWireM.h> // instead of Wire.h which won't work for ATtiny85
#include "LiquidCrystal_PCF8574_ATtiny85.h"

// Ultrasonic Sensor HC-SR04
const int trig_pin = 4;
const int echo_pin = 3;

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

int read_distance_in_cm() {
    // Clears the trig_pin
    digitalWrite(trig_pin, LOW);
    delayMicroseconds(2);

    // Sets the trig_pin on HIGH state for 10 microseconds
    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_pin, LOW);

    // Reads the echo_pin, returns the sound wave travel time in microseconds
    long duration = pulseIn(echo_pin, HIGH);

    // Calculating the distance in cm
    int distance = duration * 0.034 / 2;

    return distance;
}

void lcd_show_lines(const String& s0, const String& s1) {
    lcd.setCursor(0, 1);
    lcd.print(s0);

    lcd.setCursor(0, 2);
    lcd.print(s1);
}


void loop() {
    int dist = read_distance_in_cm();

    lcd_show_lines("----------", String(dist) + "cm        ");
}
