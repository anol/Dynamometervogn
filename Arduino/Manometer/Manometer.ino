
const int pin_a = A2;
const int pin_b = A4;

void setup() {
    Serial.begin(115200);
    Serial.println(";MANOMETER;");
    delay(1000);
}

void loop() {
    Serial.print("a=");
    Serial.print(analogRead(pin_a));
    Serial.println(";");
    Serial.print("b=");
    Serial.print(analogRead(pin_b));
    Serial.println(";");
    delay(100);
}
