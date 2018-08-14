#include "Arduino.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

uint8_t north_west[8]  = {
    0b10000,
    0b11111,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b00000
};

uint8_t north[8]  = {
    0b00000,
    0b11111,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000
};

uint8_t north_east[8]  = {
    0b00001,
    0b11111,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00000
};

uint8_t west_middle[8]  = {
    0b10000,
    0b10000,
    0b11111,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b11111
};

uint8_t west_middle_1[8]  = {
    0b10000,
    0b10000,
    0b11111,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b11111
};

uint8_t east_middle[8]  = {
    0b00001,
    0b00001,
    0b11111,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b11111
};

uint8_t south_west[8]  = {
    0b10000,
    0b11111,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b11111,
    0b10000
};

uint8_t south_east[8]  = {
    0b00001,
    0b11111,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b11111,
    0b00001
};

uint8_t north_west_south_0[8]  = {
    0b11111,
    0b10000,
    0b10111,
    0b10111,
    0b10111,
    0b10000,
    0b11111,
    0b10000,
};

uint8_t north_west_south_1[8]  = {
    0b10000,
    0b10000,
    0b11111,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b11111
};

uint8_t north_east_south_0[8]  = {
    0b11111,
    0b00001,
    0b11101,
    0b11101,
    0b11101,
    0b00001,
    0b11111,
    0b00001,
};

uint8_t north_east_south_1[8]  = {
    0b00001,
    0b00001,
    0b11111,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b11111
};

uint8_t north_south_0[8]  = {
    0b11111,
    0b00000,
    0b11111,
    0b11111,
    0b11111,
    0b00000,
    0b11111,
    0b00000,
};

uint8_t north_south_1[8]  = {
    0b00000,
    0b00000,
    0b11111,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b11111
};


LiquidCrystal_I2C lcd(0x27, 20, 4);

int i = 0;
int j = 0;

void move_to(int row, int col) {
    i = row;
    j = col;
}

void show(int row, int col, uint8_t* matrix, int char_index) {
    lcd.createChar(char_index, matrix);
    lcd.setCursor(col, row);
    lcd.write(char_index);
}

void show(uint8_t* matrix, int char_index) {
    show(i, j++, matrix, char_index);
}

void show_gate(int x, int y) {
    move_to(x, y);

    show(north_west_south_0, 0);
    show(north_south_0, 1);
    show(north_south_0, 1);
    show(north_east_south_0, 2);
    show(north_west_south_0, 0);
    show(north_south_0, 1);
    show(north_south_0, 1);
    show(north_east_south_0, 2);

    move_to(x + 1, y);

    show(west_middle, 3);
    show(north_south_1, 7);
    show(north_south_1, 7);
    show(east_middle, 4);
    show(west_middle, 3);
    show(north_south_1, 7);
    show(north_south_1, 7);
    show(east_middle, 4);

    move_to(x + 2, y);

    show(west_middle, 3);
    show(north_south_1, 7);
    show(north_south_1, 7);
    show(east_middle, 4);
    show(west_middle, 3);
    show(north_south_1, 7);
    show(north_south_1, 7);
    show(east_middle, 4);

    move_to(x + 3, y);

    show(north_west_south_1, 5);
    show(north_south_1, 7);
    show(north_south_1, 7);
    show(north_east_south_1, 6);
    show(north_west_south_1, 5);
    show(north_south_1, 7);
    show(north_south_1, 7);
    show(north_east_south_1, 6);
}

void setup() {
    lcd.init();
    lcd.backlight();

    show_gate(0, 0);
    show_gate(0, 10);
}

void loop() {
}
