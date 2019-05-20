#include <Arduino.h>
#include <EEPROM.h>

const uint EEPROM_SIZE = 512;

void blink(uint sec, uint hz) {
    uint n = hz * sec;
    uint blink_delay = 1000 / hz / 2;

    for (uint i = 0; i < n; i++) {
        digitalWrite(LED_BUILTIN, LOW);   // LOW means LED is on because it is active low on the ESP-01
        delay(blink_delay);
        digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
        delay(blink_delay);
    }
}

void eeprom_read_all(byte buf[]) {
    EEPROM.begin(EEPROM_SIZE);

    for (uint i = 0; i < EEPROM_SIZE; i++) {
        buf[i] = EEPROM.read(i);
    }

    EEPROM.end();

    blink(1, 5);  // blink on finish
}

void eeprom_write_all(const byte buf[], size_t n) {
    EEPROM.begin(EEPROM_SIZE);

    const byte padding = 0;
    for (uint i = 0; i < EEPROM_SIZE; i++) {
        byte b = i < n ? buf[i] : padding;
        EEPROM.write(i, b);
    }

    EEPROM.commit();
    EEPROM.end();

    blink(5, 1);  // blink on finish
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
    Serial.print((char*)eeprom_all);    // what's is? 78b4e04d81022324f932273ce2847e44
                                        //  and this? 2420ab8f0523fe5b2c6672735e5180ee

    Serial.println();
    Serial.println("==========");
}

void setup() {
    delay(10000); // so that I have plenty of time to open serial monitor

    Serial.begin(115200);

    pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output

    serial_print_eeprom_all();

    delay(1000);

    const char * s = "SSID_AS_AP= "
                     "PASSWORD_AS_AP= "
                     "SSID= "
                     "PASSWORD= "
                     "HUE_AUTH=";
    // eeprom_write_all((byte*)s, strlen(s) + 1);

    delay(1000);

    serial_print_eeprom_all();
}

void loop() {
    delay(1000);
}
