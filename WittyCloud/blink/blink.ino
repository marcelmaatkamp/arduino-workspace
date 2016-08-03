#include "Streaming.h"

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  Serial << "setup\n";
}

void blink(int pin) {
  Serial << "analogRead(A0)=" << analogRead(A0) << "\n";

  pinMode(pin, OUTPUT);
  Serial << "pinMode(" << pin << ",OUTPUT)\n";
  delay(1000);

  digitalWrite(pin, 0);
  Serial << "digitalWrite(" << pin << ",0)\n";
  delay(1000);

  digitalWrite(pin, 1);
  Serial << "digitalWrite(" << pin << ",1)\n";
  delay(1000);

  for (int pct = 10; pct <= 90; pct += 10) {
    analogWrite(pin, static_cast<int>(PWMRANGE * 0.01 * pct));
    Serial << "analogWrite(" << pin << "," << pct << "%)\n";
    delay(1000);
  }

  analogWrite(pin, 0);
}

void loop() {
  blink(0);
  blink(2);
  blink(4);
  blink(5);
  blink(12);
  blink(13);
  blink(14);
  blink(15);
  blink(16);
}
