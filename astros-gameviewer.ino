#include "src/display.h"

void setup() {
  Serial.begin(9600);
  displayInit();
  displayUpdate("Astros Live", "HOU 7 - TEX 4");
}

void loop() {
  // Main game viewer loop
}
