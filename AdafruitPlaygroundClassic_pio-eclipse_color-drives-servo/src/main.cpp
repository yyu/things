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

enum COLOR {
    RED, BLUE, NO_IDEA
};

// no idea how the pins are numbered
constexpr int SHARP6  = 25; // pin #6  ==  7 == 25
constexpr int SHARP12 = 29; // pin #12 == 11 == 29
constexpr int SHARP9  = 27; // pin #9  ==  9 == 27
constexpr int SHARP10 = 14; // pin #10 == 10 == 14

constexpr int   POT_PIN = SHARP6; // the potentiometer
constexpr int SERVO_PIN = SHARP12;

constexpr int LEFTMOST_ANGLE = 110;
constexpr int MIDDLE_ANGLE = 85;
constexpr int RIGHTMOST_ANGLE = 60;

constexpr int SERVO_MOVE_WAIT_TIME = 1000; // in milliseconds
constexpr int LED_BLINK_INTERVAL = 500; // in milliseconds

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
        delay(LED_BLINK_INTERVAL);
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

void wait_for_servo_to_get_there() {
    delay(SERVO_MOVE_WAIT_TIME);
}

void turn_back_middle() {
    myservo.write(MIDDLE_ANGLE);
}

void turn_left() {
    myservo.write(LEFTMOST_ANGLE);
}

void turn_right() {
    myservo.write(RIGHTMOST_ANGLE);
}

void swipe_left() {
    turn_left();
    wait_for_servo_to_get_there();
    turn_back_middle();
    wait_for_servo_to_get_there();
}

void swipe_right() {
    turn_right();
    wait_for_servo_to_get_there();
    turn_back_middle();
    wait_for_servo_to_get_there();
}

void swipe(bool left_0_right_1) {
    if (left_0_right_1) {
        swipe_right();
    } else {
        swipe_left();
    }
}

COLOR get_color() {
    // First turn off all the pixels to make sure they don't interfere with the reading.
    CircuitPlayground.clearPixels();

    // color reading (red, green, blue will be returned in the parameters passed in)
    uint8_t red, green, blue;
    CircuitPlayground.senseColor(red, green, blue);

    // Print out the color components.
    Serial.print("Color: red=");
    Serial.print(red, DEC);
    Serial.print(" green=");
    Serial.print(green, DEC);
    Serial.print(" blue=");
    Serial.println(blue, DEC);

    // Finally set all the pixels to the detected color.
    for (int i=0; i<10; ++i) {
      CircuitPlayground.strip.setPixelColor(i, red, green, blue);
    }
    CircuitPlayground.strip.show();

    if (red > green && red > blue) {
        return RED;
    } else if (blue > red && blue > green) {
        return BLUE;
    } else {
        return NO_IDEA;
    }
}

void read_pot_and_swipe() {
    int pot_val = analogRead(POT_PIN); // reads the value of the potentiometer (value between 0 and 1023)
    int servo_angle = map(pot_val, 0, 1023, 0, 180); // scale it to use it with the servo (value between 0 and 180)
    bool left_false_right_true = servo_angle > 90;

    SERIAL_PRINT_5_LN("pot: ", pot_val, " => angle: ", servo_angle, left_false_right_true ? " (Right)" : " (Left)");

    swipe(left_false_right_true);
}

void read_color_and_swipe() {
    const COLOR color = get_color();

    switch (color) {
        case RED:
            swipe_left();
            break;
        case BLUE:
            swipe_right();
            break;
        default:
            break;
    }
    SERIAL_PRINT_5("(RED, BLUE, NO_IDEA): (", RED, ", ", BLUE, ", ");
    SERIAL_PRINT_5_LN(NO_IDEA, "). ", "read: ", color, ".");
}

void loop() {
    //check_many_pins();

    read_color_and_swipe();

    blink_led();
}
