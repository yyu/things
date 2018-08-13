#include "Arduino.h"
#include <TinyWireM.h>
//#include "LiquidCrystal_I2C_ATTiny85.h"
#include "LiquidCrystal_PCF8574_ATtiny85.h"

// Ultrasonic Sensor HC-SR04
const int trigPin = 4;
const int echoPin = 3;

#define I2C_ADDRESS 0x27
const int WIDTH = 20;
const int HEIGHT = 4;
//LiquidCrystal_I2C_ATTiny85 lcd(I2C_ADDRESS, WIDTH, HEIGHT);
LiquidCrystal_PCF8574_ATtiny85 lcd(I2C_ADDRESS);

int good = true;

void setup() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

//    TinyWireM.begin();
    lcd.begin(WIDTH, HEIGHT); // initialize the lcd

    lcd.setBacklight(255);
    lcd.home(); lcd.clear();
//    lcd.print("Hello LCD");

//    lcd.init();
//    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Distance:");
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
//    lcd.setBacklight(255);
//    lcd.home();
//    lcd.clear();

    lcd.setCursor(0, 1);
    lcd.print(s0);

    lcd.setCursor(0, 2);
    lcd.print(s1);
}


void loop() {
    int dist = read_distance_in_cm();

    lcd_show_lines("----------", String(dist) + "cm        ");
}

