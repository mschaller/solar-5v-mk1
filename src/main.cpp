#include <Arduino.h>

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  for(int i = 0; i < 40; i++) {
    digitalWrite(LED_BUILTIN, 1);
    delay(10);
    digitalWrite(LED_BUILTIN, 0);
    delay(30);
  }

  Serial.begin(115200);
  delay(10);

  Serial.println(F("solar-5v-mk1"));
}

void loop() {
  // put your main code here, to run repeatedly:
    digitalWrite(LED_BUILTIN, 1);
    delay(4900);
    digitalWrite(LED_BUILTIN, 0);
    delay(100);

    float a0 = analogRead(A0);

    Serial.print("A0: ");
    Serial.print(a0);
    Serial.print("  Vin: ");
    Serial.println(a0 * 0.004666666667);


}