
const int pin_a = A2;
const int pin_b = A4;

int old_A = 0;
int old_B = 0;

void setup() {
    Serial.begin(115200);
    Serial.println(";MANOMETER;");
    delay(1000);
}

void loop() {
    int val_A = analogRead(pin_a);
    int val_B = analogRead(pin_b);
    if (val_A != old_A) {
        old_A = val_A;
        Serial.print("a=");
        Serial.print(val_A);
        Serial.println(";");
    }
    if (val_B != old_B) {
        old_B = val_B;
        Serial.print("b=");
        Serial.print(val_B);
        Serial.println(";");
    }
    delay(100);
}
