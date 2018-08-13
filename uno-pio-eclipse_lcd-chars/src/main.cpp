#include "Arduino.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

uint8_t c0[8]  = {
    0b11111,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000
};

uint8_t c1[8]  = {
    0b11111,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000
};

uint8_t c2[8]  = {
    0b11111,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001
};

uint8_t c3[8]  = {
    0b10000,
    0b10000,
    0b10000,
    0b11111,
    0b10000,
    0b10000,
    0b10000,
    0b10000
};

uint8_t c4[8]  = {
    0b00001,
    0b00001,
    0b00001,
    0b11111,
    0b00001,
    0b00001,
    0b00001,
    0b00001
};

uint8_t c5[8]  = {
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b11111
};

uint8_t c6[8]  = {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b11111
};

uint8_t c7[8]  = {
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b11111
};

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup()
{
  lcd.init();
  lcd.backlight();

    lcd.createChar(0, c0);
    lcd.createChar(1, c1);
    lcd.createChar(2, c2);
    lcd.createChar(3, c3);
    lcd.createChar(4, c4);
    lcd.createChar(5, c5);
    lcd.createChar(6, c6);
    lcd.createChar(7, c7);
    lcd.home();

    lcd.print("Hello world...");

    lcd.setCursor(0, 1);

    lcd.write(byte(0));
    for (int i = 0; i < 10; i++) {
        lcd.write(byte(1));
    }
    lcd.write(byte(2));

    lcd.setCursor(0, 2);

    lcd.write(byte(3));
    for (int i = 0; i < 10; i++) {
        lcd.write('-');
    }
    lcd.write(byte(4));

    lcd.setCursor(0, 3);

    lcd.write(byte(5));
    for (int i = 0; i < 10; i++) {
        lcd.write(byte(6));
    }
    lcd.write(byte(7));
}

void loop() {
}
