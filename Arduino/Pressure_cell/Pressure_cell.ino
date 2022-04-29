
const int pin_a = A2;
const int pin_b = A4;

void setup() {
    Serial.begin(115200);
    delay(10);
    Serial.println("Setup done;");
    Serial.println("x=P;");
}

void loop() {
    Serial.print("a=");
    Serial.print(analogRead(pin_a));
    Serial.print(";b=");
    Serial.print(analogRead(pin_b));
    Serial.print(";\r");
    delay(100);
}
