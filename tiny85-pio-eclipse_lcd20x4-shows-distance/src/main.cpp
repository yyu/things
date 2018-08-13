#include "Arduino.h"
#include <TinyWireM.h>
//#include "LiquidCrystal_I2C_ATTiny85.h"
#include "LiquidCrystal_PCF8574_ATtiny85.h"

#define I2C_ADDRESS 0x27
const int WIDTH = 20;
const int HEIGHT = 4;
//LiquidCrystal_I2C_ATTiny85 lcd(I2C_ADDRESS, WIDTH, HEIGHT);
LiquidCrystal_PCF8574_ATtiny85 lcd(I2C_ADDRESS);

int good = true;

void setup() {
//    TinyWireM.begin();
    lcd.begin(WIDTH, HEIGHT); // initialize the lcd

    lcd.setBacklight(255);
    lcd.home(); lcd.clear();
//    lcd.print("Hello LCD");

//    lcd.init();
//    lcd.backlight();
    lcd.setCursor(0, 1);
    lcd.print("Hello ATtiny85");
}


void loop() {
    if (!good) {
        digitalWrite(0, HIGH);
        delay(500);
        digitalWrite(0, LOW);
        delay(500);
    }
}

