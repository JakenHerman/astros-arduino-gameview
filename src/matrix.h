#ifndef MATRIX_H
#define MATRIX_H

#include <MD_MAX72xx.h>
#include <SPI.h>
#define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW 
#define MAX_DEVICES   1
#define DIN_PIN       50
#define CS_PIN        52
#define CLK_PIN       53

extern MD_MAX72XX mx;

void matrixInit();
void clearMatrix();
void showRunners(bool on1B, bool on2B, bool on3B);
void testMatrixPattern();
void showInningAndRunners(int inning, bool on1B, bool on2B, bool on3B);
#endif
