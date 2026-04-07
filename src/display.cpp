#include "display.h"
#include <Arduino.h>
#include <LiquidCrystal.h>

// RS, EN, D4, D5, D6, D7
static LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

void displayInit() {
  lcd.begin(16, 2);
  lcd.clear();
}

void displayUpdate(const char* line1, const char* line2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}

void displayClear() {
  lcd.clear();
}
