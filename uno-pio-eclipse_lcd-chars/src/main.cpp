#include "Arduino.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

uint8_t c0[8]  = {
    0b11111,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10001
};

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup()
{
  lcd.init();
  lcd.backlight();

  lcd.createChar(0, c0);
  lcd.home();

  lcd.print("Hello world...");
  lcd.setCursor(0, 1);
  lcd.write(0);

}

void loop()
{

}
