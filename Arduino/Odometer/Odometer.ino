const byte ledPin = 13;
const byte interruptPinA = 2;
const byte interruptPinB = 3;
volatile byte state = LOW;

void setup() {
    pinMode(ledPin, OUTPUT);
    pinMode(interruptPinA, INPUT_PULLUP);
    pinMode(interruptPinB, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interruptPinA), blinkA, CHANGE);
    attachInterrupt(digitalPinToInterrupt(interruptPinB), blinkB, CHANGE);
}

void loop() {
    digitalWrite(ledPin, state);
}

void blinkA() {
    state = !state;
}
void blinkB() {
    state = !state;
}