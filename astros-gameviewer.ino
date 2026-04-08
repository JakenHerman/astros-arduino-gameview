#include "src/display.h"
#include "src/matrix.h"
#include <ArduinoJson.h>

// ====================== LED PINS ======================
const int LED_PINS[] = {40, 41, 42, 43, 44, 45, 46, 47, 48, 49};
const int LED_COUNT = 10;

// ====================== SCOREBOARD STATE ======================
int homeRuns = 0;
int awayRuns = 0;
int currentInning = 1;
bool isTopInning = true;
int currentBalls = 0;
int currentStrikes = 0;
int currentOuts = 0;
bool on1B = false;
bool on2B = false;
bool on3B = false;

String inputBuffer = "";

// ====================== LED HELPER FUNCTIONS ======================
void initLEDs() {
  for (int i = 0; i < LED_COUNT; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }
}

void setBalls(int count) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(LED_PINS[i], (i < count) ? HIGH : LOW);
  }
}

void setStrikes(int count) {
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_PINS[4 + i], (i < count) ? HIGH : LOW);
  }
}

void setOuts(int count) {
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_PINS[7 + i], (i < count) ? HIGH : LOW);
  }
}

// ====================== SERIAL DATA HANDLER ======================
void handleSerialData() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      StaticJsonDocument<512> doc;
      DeserializationError error = deserializeJson(doc, inputBuffer);

      if (!error) {
        homeRuns = doc["home_runs"] | 0;
        awayRuns = doc["away_runs"] | 0;
        currentInning = doc["inning"] | 1;
        isTopInning = doc["top"] | true;
        currentBalls = doc["balls"] | 0;
        currentStrikes = doc["strikes"] | 0;
        currentOuts = doc["outs"] | 0;
        on1B = doc["on1b"] | false;
        on2B = doc["on2b"] | false;
        on3B = doc["on3b"] | false;

        // Update LCD
        String topLine = "HOU " + String(homeRuns) + " - " + String(awayRuns);
        String bottomLine = (isTopInning ? "Top " : "Bot ") + String(currentInning) + "   O:" + String(currentOuts);
        displayUpdate(topLine.c_str(), bottomLine.c_str());

        // Update LEDs
        setBalls(currentBalls);
        setStrikes(currentStrikes);
        setOuts(currentOuts);

        // Update Matrix
        showInningAndRunners(currentInning, on1B, on2B, on3B);

        Serial.println("✅ Live update received");
      } else {
        Serial.print("JSON error: ");
        Serial.println(error.c_str());
      }

      inputBuffer = "";
    } else {
      inputBuffer += c;
    }
  }
}

// ====================== SETUP ======================
void setup() {
  Serial.begin(9600);

  displayInit();
  initLEDs();
  matrixInit();

  // Starting screen
  displayUpdate("Astros Live", "Waiting for data...");
  showInningAndRunners(-1, false, false, false);

  Serial.println("Astros Scoreboard v1.0 ready - waiting for Python data...");
}

// ====================== LOOP ======================
void loop() {
  handleSerialData();     // Process incoming JSON from Python
  delay(50);
}
