const byte pin_LED = 13;
const byte pin_A = 2;
const byte pin_B = 3;
volatile byte state = 0;
volatile byte toggle_LED = LOW;
volatile int trip = 0;
String my_command = NULL;

// Arduino Uno
#define digitalPinToInterrupt(p) ( (p) == 2 ? 0 : ((p) == 3 ? 1 : -1) )

void setup() {
    Serial.begin(115200);
    pinMode(pin_LED, OUTPUT);
    pinMode(pin_A, INPUT_PULLUP);
    pinMode(pin_B, INPUT_PULLUP);
    Serial.println("O=ODOMETER;");
    delay(1000);
    attachInterrupt(digitalPinToInterrupt(pin_A), blink, CHANGE);
    attachInterrupt(digitalPinToInterrupt(pin_B), blink, CHANGE);
    Serial.setTimeout(1000);
}

void loop() {
    delay(1000);
    my_command = Serial.readStringUntil(';');
    if(my_command != NULL){
        Serial.println("O=ODOMETER;");
        my_command = NULL;
    }
    toggle_LED = !toggle_LED;
    digitalWrite(pin_LED, toggle_LED);
}

void blink() {
    state <<= 2;
    state |= digitalRead(pin_A) == LOW ? 0 : 1;
    state |= digitalRead(pin_B) == LOW ? 0 : 2;
    switch (state) {
        case 130:
        case 180: {
            trip ++;
            Serial.print("F=");
            Serial.print(trip);
            Serial.println(";");
            break;
        }
        case 120:
        case 184: {
            trip --;
            Serial.print("B=");
            Serial.print(trip);
            Serial.println(";");
            break;
        }
        default: {
//            Serial.print("o=");
//            Serial.print(state, BIN);
//            Serial.println(";");
            break;
        }
    }
}
