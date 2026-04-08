#include "matrix.h"

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DIN_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

void matrixInit() {
  mx.begin();
  mx.control(MD_MAX72XX::INTENSITY, 8);
  clearMatrix();
  Serial.println("Matrix initialized OK");
}

void clearMatrix() {
  mx.clear();
}

void drawBaselines() {
  mx.setPoint(5, 3, true);
  mx.setPoint(3, 1, true);
  mx.setPoint(1, 3, true);
  mx.setPoint(3, 5, true);
  mx.setPoint(2, 2, true);
  mx.setPoint(4, 2, true);
  mx.setPoint(4, 4, true);
  mx.setPoint(2, 4, true);
}

void showRunners(bool on1B, bool on2B, bool on3B) {
  mx.clear();
  drawBaselines();

  if (on1B) mx.setPoint(6, 3, true);   // 1B
  if (on2B) mx.setPoint(3, 0, true);   // 2B
  if (on3B) mx.setPoint(0, 3, true);   // 3B
}

void showInningAndRunners(int inning, bool on1B, bool on2B, bool on3B) {
  mx.clear();
  
  // Show inning number
  for(int i = 0; i <= inning; i++) {
    if (i > 7) {
      mx.setPoint(i - 8, 6, true);
    } else {
      mx.setPoint(i, 7, true);
    }
  }

  // Draw diamond + runners
  drawBaselines();
  if (on1B) mx.setPoint(6, 3, true);
  if (on2B) mx.setPoint(3, 0, true);
  if (on3B) mx.setPoint(0, 3, true);
}
