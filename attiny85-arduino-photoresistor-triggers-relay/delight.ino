const int inputPin = A1;
const int relayPin = 1;

void setup() {
  pinMode(inputPin, INPUT);
  pinMode(relayPin, OUTPUT);
}

void loop() {
  int x = analogRead(inputPin);
  if (x > 100) {
    digitalWrite(relayPin, HIGH);
  } else {
    digitalWrite(relayPin, LOW);
  }
  delay(1000);
}
