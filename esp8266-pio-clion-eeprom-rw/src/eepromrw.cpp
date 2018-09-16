#include <Arduino.h>
#include <EEPROM.h>

const uint BLINK_HZ = 5;
const uint BLINK_DELAY = 1000 / BLINK_HZ / 2;

const uint EEPROM_SIZE = 512;

void blink(uint sec) {
    uint n = BLINK_HZ * sec;

    for (uint i = 0; i < n; i++) {
        digitalWrite(LED_BUILTIN, LOW);   // LOW means LED is on because it is active low on the ESP-01
        delay(BLINK_DELAY);
        digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
        delay(BLINK_DELAY);
    }
}

void eeprom_read_all(byte buf[]) {
    EEPROM.begin(EEPROM_SIZE);

    for (uint i = 0; i < EEPROM_SIZE; i++) {
        buf[i] = EEPROM.read(i);
    }

    blink(1);  // blink on finish

    EEPROM.end();
}
void eeprom_write_all(byte buf[]) {
    EEPROM.begin(EEPROM_SIZE);

    for (uint i = 0; i < EEPROM_SIZE; i++) {
        EEPROM.write(i, buf[i]);
    }

    blink(3);  // blink on finish

    EEPROM.end();
}

void serial_print_eeprom_all() {
    byte eeprom_all[EEPROM_SIZE + 1];

    eeprom_read_all(eeprom_all);

    for (uint i = 0; i < EEPROM_SIZE; i++) {
        Serial.print(i % 32 == 0 ? '\n' : ' ');
        Serial.print(eeprom_all[i], 16);
    }
    Serial.println();
    Serial.println("--");

    eeprom_all[EEPROM_SIZE] = '\0';
    Serial.print((char*)eeprom_all);  // what's is? 78b4e04d81022324f932273ce2847e44

    Serial.println("==========");
}

void setup() {
    delay(10000); // so that I have plenty of time to open serial monitor

    Serial.begin(115200);

    pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output

    serial_print_eeprom_all();
}

void loop() {
    delay(1000);
}
