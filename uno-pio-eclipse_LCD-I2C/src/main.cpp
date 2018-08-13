/*
 * https://www.sunfounder.com/iic-i2c-twi-serial-2004-20x4-lcd-module-shield-for-arduino-uno-mega2560.html
 */

#include "Arduino.h"

#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

#define I2C_ADDRESS 0x27

const int WIDTH = 20;
const int HEIGHT = 4;

LiquidCrystal_PCF8574 lcd(I2C_ADDRESS);

int show;

void setup()
{
  int error;

  Serial.begin(115200);
  Serial.println("LCD...");

  while (!Serial);

  Serial.println("Dose: check for LCD");

  // See http://playground.arduino.cc/Main/I2cScanner
  Wire.begin();
  Wire.beginTransmission(I2C_ADDRESS);
  error = Wire.endTransmission();
  Serial.print("Error: ");
  Serial.print(error);

  if (error == 0) {
    Serial.println(": LCD found.");

  } else {
    Serial.println(": LCD not found.");
  } // if

  lcd.begin(WIDTH, HEIGHT); // initialize the lcd
  show = 0;
} // setup()

void loop()
{
  if (show == 0) {
    lcd.setBacklight(255);
    lcd.home(); lcd.clear();
    lcd.print("Hello LCD");
    delay(1000);

    lcd.setBacklight(0);
    delay(400);
    lcd.setBacklight(255);

  } else if (show == 1) {
    lcd.clear();
    lcd.print("Cursor On");
    lcd.cursor();

  } else if (show == 2) {
    lcd.clear();
    lcd.print("Cursor Blink");
    lcd.blink();

  } else if (show == 3) {
    lcd.clear();
    lcd.print("Cursor OFF");
    lcd.noBlink();
    lcd.noCursor();

  } else if (show == 4) {
    lcd.clear();
    lcd.print("Display Off");
    lcd.noDisplay();

  } else if (show == 5) {
    lcd.clear();
    lcd.print("Display On");
    lcd.display();

  } else if (show == 7) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("*** first line.");
    lcd.setCursor(0, 1);
    lcd.print("*** second line.");
    lcd.setCursor(0, 2);
    lcd.print("*** third line.");
    lcd.setCursor(0, 3);
    lcd.print("*** fourth line.");

  } else if (show == 8) {
    lcd.scrollDisplayLeft();
  } else if (show == 9) {
    lcd.scrollDisplayLeft();
  } else if (show == 10) {
    lcd.scrollDisplayLeft();
  } else if (show == 11) {
    lcd.scrollDisplayRight();
  } // if

  delay(2000);
  show = (show + 1) % 12;
} // loop()

