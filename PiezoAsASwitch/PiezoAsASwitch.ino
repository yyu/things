const int VALVE_SWITCH = 9;
const int UNLOAD_SWITCH = 5;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(VALVE_SWITCH, OUTPUT);
  pinMode(UNLOAD_SWITCH, OUTPUT);

  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  int piezoADC = analogRead(A0);
  Serial.println(piezoADC);

  if (piezoADC > 20) {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(VALVE_SWITCH, HIGH);
    digitalWrite(UNLOAD_SWITCH, HIGH);
    delay(5000);
  } else {
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(VALVE_SWITCH, LOW);
    digitalWrite(UNLOAD_SWITCH, LOW);
    delay(1000);
  }
}