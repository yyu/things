#include <Adafruit_CircuitPlayground.h>

/*
    "#undef MAX_SERVOS" so that the compiler no longer complains

    In file included from src/main.cpp:2:0:
    /Users/yy/.platformio/packages/framework-arduinoavr/libraries/Servo/src/Servo.h:84:0: warning: "MAX_SERVOS" redefined
    #define MAX_SERVOS   (_Nbr_16timers  * SERVOS_PER_TIMER)
    ^
    In file included from .piolibdeps/Adafruit Circuit Playground_ID602/utility/CP_Firmata.h:17:0,
    from .piolibdeps/Adafruit Circuit Playground_ID602/Adafruit_Circuit_Playground.h:25,
    from .piolibdeps/Adafruit Circuit Playground_ID602/Adafruit_CircuitPlayground.h:3,
    from src/main.cpp:1:
    .piolibdeps/Adafruit Circuit Playground_ID602/utility/CP_Boards.h:32:0: note: this is the location of the previous definition
    #define MAX_SERVOS 0
    ^
 */
#undef MAX_SERVOS

#include <Servo.h>

// no idea how the pins are numbered
constexpr int SHARP6  = 25; // pin #6  ==  7 == 25
constexpr int SHARP12 = 29; // pin #12 == 11 == 29
constexpr int SHARP9  = 27; // pin #9  ==  9 == 27
constexpr int SHARP10 = 14; // pin #10 == 10 == 14

constexpr int   POT_PIN = SHARP6; // the potentiometer
constexpr int SERVO_PIN = SHARP12;

Servo myservo;  // create servo object to control a servo

template<typename A, typename B, typename C, typename D, typename E>
constexpr void SERIAL_PRINT_5(const A a, const B& b, const C& c, const D& d,
        const E& e) {
    Serial.print(a);
    Serial.print(b);
    Serial.print(c);
    Serial.print(d);
    Serial.print(e);
}

template<typename A, typename B, typename C, typename D, typename E>
constexpr void SERIAL_PRINT_5_LN(const A a, const B& b, const C& c, const D& d,
        const E& e) {
    SERIAL_PRINT_5(a, b, c, d, e);
    Serial.println();
}

void setup() {
    CircuitPlayground.begin();

    Serial.begin(9600);

    myservo.attach(SERVO_PIN);
}

void blink_led() {
    int cycle[] = { HIGH, LOW };

    for (int high_or_low : cycle) {
        CircuitPlayground.redLED(high_or_low);
        delay(500);
    }
}

// used this to figure out the pin numbers; stupid but works
void check_many_pins() {
    for (int i = 0; i < 30; i++) {
        int val = analogRead(i);
        SERIAL_PRINT_5("[", i, "]", val, ".");
    }
    Serial.println();
}

void turn_back_middle() {
    myservo.write(85);
}

void turn_left() {
    myservo.write(135);
}

void turn_right() {
    myservo.write(35);
}

void loop() {
    //check_many_pins();

    int pot_val = analogRead(POT_PIN); // reads the value of the potentiometer (value between 0 and 1023)
    int servo_angle = map(pot_val, 0, 1023, 0, 180); // scale it to use it with the servo (value between 0 and 180)

    SERIAL_PRINT_5_LN("pot: ", pot_val, " => ", "angle: ", servo_angle);

    // myservo.write(servo_angle); // sets the servo position according to the scaled value

    if (servo_angle <= 90) {
        turn_left();
        delay(1000);
        turn_back_middle();
        delay(1000);
    } else {
        turn_right();
        delay(1000);
        turn_back_middle();
        delay(1000);
    }

    delay(200); // waits for the servo to get there

    blink_led();
}
