#include "src/display.h"

const int LED_PINS[] = {40, 41, 42, 43, 44, 45, 46, 47, 48, 49};
const int LED_COUNT = 10;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < LED_COUNT; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], HIGH);
  }
  displayInit();
  displayUpdate("Astros Live", "HOU 7 - TEX 4");
}

void loop() {
  // Software PWM ~30% brightness
  const unsigned long period = 10000;
  const unsigned long duty   =  3000;
  unsigned long t = micros() % period;
  bool on = (t < duty);
  for (int i = 0; i < LED_COUNT; i++) {
    digitalWrite(LED_PINS[i], on ? HIGH : LOW);
  }
}
