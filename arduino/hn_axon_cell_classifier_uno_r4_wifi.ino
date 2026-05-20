/*
  3x3 LDR LIGHT DIRECTION CLASSIFIER

  Hardware:
  - Arduino UNO R4 WiFi
  - Built-in 12x8 LED matrix
  - 9 LDR sensors
  - CD4051B multiplexer for N1-N8
  - N9 directly connected to A1

  Physical layout:

        N3    N2    N1
        N4    N7    N8
        N9    N5    N6

  Main fix in this version:
  - N9 scaling is NOT applied during baseline calibration.
  - N9 scaling is applied only after baselineReady = true.
*/

#include "Arduino_LED_Matrix.h"
#include <math.h>

ArduinoLEDMatrix matrix;

// Pins
const int S0 = 2;
const int S1 = 3;
const int S2 = 4;

const int MUX_SIG = A0;
const int SENSOR9 = A1;

// Settings
const int NUM_SENSORS = 9;

const int BASELINE_SAMPLES = 100;
const int TEMPLATE_SAMPLES = 200;
const int TEMPLATE_DELAY = 50;

const int LIVE_DELAY = 90;

const float ACTIVITY_THRESHOLD = 50.0;
const float N9_SCALE = 0.50;

const int HISTORY_LEN = 17;

// Data
int values[NUM_SENSORS];

float baseline[NUM_SENSORS];
float livePattern[NUM_SENSORS];
float templates[NUM_SENSORS][NUM_SENSORS];

int winnerHistory[HISTORY_LEN];
int historyIndex = 0;

uint8_t frame[8][12];

// Important: prevents N9 compensation from corrupting baseline.
bool baselineReady = false;

// Internal index mapping:
// N1=0, N2=1, N3=2, N4=3, N5=4, N6=5, N7=6, N8=7, N9=8

int calibrationOrder[9] = {
  2, 1, 0,
  3, 6, 7,
  8, 4, 5
};

const char* names[9] = {
  "N1_UP_RIGHT",
  "N2_UP",
  "N3_UP_LEFT",
  "N4_LEFT",
  "N5_DOWN",
  "N6_DOWN_RIGHT",
  "N7_CENTER",
  "N8_RIGHT",
  "N9_DOWN_LEFT"
};

void setup() {
  Serial.begin(115200);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);

  matrix.begin();

  resetHistoryToCenter();
  drawIdle();

  delay(1500);

  Serial.println("=== 3x3 LDR DIRECTION CLASSIFIER ===");
  Serial.println("Step 1: Keep flashlight away.");
  Serial.println("Calibrating ambient baseline...");

  calibrateBaseline();

  Serial.println();
  Serial.println("Step 2: Template calibration begins.");
  Serial.println("Move flashlight to the dot shown on the LED matrix.");
  Serial.println("You will get a 5 second countdown.");
  Serial.println("Then 10 seconds of data are recorded.");

  delay(2000);

  calibrateTemplates();

  Serial.println();
  Serial.println("Calibration complete.");
  Serial.println("Live classification started.");
  Serial.println("time_ms,N1,N2,N3,N4,N5,N6,N7,N8,N9,activity,rawWinner,stableWinner,score");

  drawIdle();
}

void selectChannel(int ch) {
  digitalWrite(S0, ch & 0x01);
  digitalWrite(S1, (ch >> 1) & 0x01);
  digitalWrite(S2, (ch >> 2) & 0x01);

  delayMicroseconds(100);
}

void readSensors() {
  // Read N1-N8 through CD4051.
  for (int ch = 0; ch < 8; ch++) {
    selectChannel(ch);
    values[ch] = analogRead(MUX_SIG);
  }

  // Read N9 directly.
  int rawN9 = analogRead(SENSOR9);

  /*
    Critical fix:

    During baseline calibration, baseline[8] is not valid yet.
    So we must read raw N9 without compensation.

    After baseline is calibrated, we scale only the delta:

      corrected_N9 = baseline_N9 + N9_SCALE * (raw_N9 - baseline_N9)
  */

  if (!baselineReady) {
    values[8] = rawN9;
  } else {
    values[8] = baseline[8] + N9_SCALE * (rawN9 - baseline[8]);
  }
}

void averageSensors(float out[NUM_SENSORS], int samples, int delayMs) {
  float sums[NUM_SENSORS] = {0};

  for (int k = 0; k < samples; k++) {
    readSensors();

    for (int i = 0; i < NUM_SENSORS; i++) {
      sums[i] += values[i];
    }

    delay(delayMs);
  }

  for (int i = 0; i < NUM_SENSORS; i++) {
    out[i] = sums[i] / samples;
  }
}

void calibrateBaseline() {
  baselineReady = false;

  averageSensors(baseline, BASELINE_SAMPLES, 30);

  baselineReady = true;

  Serial.print("Baseline: ");

  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print("N");
    Serial.print(i + 1);
    Serial.print("=");
    Serial.print(baseline[i], 2);
    Serial.print(" ");
  }

  Serial.println();
}

void countdown5() {
  for (int n = 5; n >= 1; n--) {
    Serial.print("Recording starts in ");
    Serial.print(n);
    Serial.println("...");
    delay(1000);
  }

  Serial.println("Recording now. Hold flashlight steady.");
}

void calibrateTemplates() {
  for (int step = 0; step < NUM_SENSORS; step++) {
    int sensorIndex = calibrationOrder[step];

    drawDotForSensor(sensorIndex);

    Serial.println();
    Serial.print("Point flashlight at ");
    Serial.print(names[sensorIndex]);
    Serial.println(".");

    countdown5();

    float avg[NUM_SENSORS];
    averageSensors(avg, TEMPLATE_SAMPLES, TEMPLATE_DELAY);

    for (int i = 0; i < NUM_SENSORS; i++) {
      templates[sensorIndex][i] = avg[i] - baseline[i];
    }

    removeCommonMode(templates[sensorIndex]);
    normalizePattern(templates[sensorIndex]);

    Serial.print("Learned template for ");
    Serial.println(names[sensorIndex]);

    delay(700);
  }
}

void removeCommonMode(float pattern[NUM_SENSORS]) {
  float mean = 0;

  for (int i = 0; i < NUM_SENSORS; i++) {
    mean += pattern[i];
  }

  mean /= NUM_SENSORS;

  for (int i = 0; i < NUM_SENSORS; i++) {
    pattern[i] -= mean;
  }
}

void normalizePattern(float pattern[NUM_SENSORS]) {
  float norm = 0;

  for (int i = 0; i < NUM_SENSORS; i++) {
    norm += pattern[i] * pattern[i];
  }

  norm = sqrt(norm);

  if (norm < 0.001) {
    return;
  }

  for (int i = 0; i < NUM_SENSORS; i++) {
    pattern[i] /= norm;
  }
}

void buildLivePattern() {
  readSensors();

  for (int i = 0; i < NUM_SENSORS; i++) {
    livePattern[i] = values[i] - baseline[i];
  }

  removeCommonMode(livePattern);
  normalizePattern(livePattern);
}

float computeActivityFromRawDelta() {
  float sumAbs = 0;

  for (int i = 0; i < NUM_SENSORS; i++) {
    sumAbs += fabs(values[i] - baseline[i]);
  }

  return sumAbs / NUM_SENSORS;
}

float similarity(float a[NUM_SENSORS], float b[NUM_SENSORS]) {
  float dot = 0;

  for (int i = 0; i < NUM_SENSORS; i++) {
    dot += a[i] * b[i];
  }

  return dot;
}

int classifyLive(float &bestScore) {
  buildLivePattern();

  int bestIndex = 0;
  bestScore = -999;

  for (int k = 0; k < NUM_SENSORS; k++) {
    float s = similarity(livePattern, templates[k]);

    if (s > bestScore) {
      bestScore = s;
      bestIndex = k;
    }
  }

  return bestIndex;
}

void resetHistoryToCenter() {
  for (int i = 0; i < HISTORY_LEN; i++) {
    winnerHistory[i] = 6; // N7 center
  }

  historyIndex = 0;
}

int getStableWinner(int newWinner) {
  winnerHistory[historyIndex] = newWinner;
  historyIndex = (historyIndex + 1) % HISTORY_LEN;

  int counts[NUM_SENSORS] = {0};

  for (int i = 0; i < HISTORY_LEN; i++) {
    int w = winnerHistory[i];

    if (w >= 0 && w < NUM_SENSORS) {
      counts[w]++;
    }
  }

  int best = 0;

  for (int i = 1; i < NUM_SENSORS; i++) {
    if (counts[i] > counts[best]) {
      best = i;
    }
  }

  return best;
}

void clearFrame() {
  for (int r = 0; r < 8; r++) {
    for (int c = 0; c < 12; c++) {
      frame[r][c] = 0;
    }
  }
}

void setPixelMapped(int r, int c) {
  int rr = 7 - r;
  int cc = 11 - c;

  if (rr >= 0 && rr < 8 && cc >= 0 && cc < 12) {
    frame[rr][cc] = 1;
  }
}

void drawDotForSensor(int index) {
  clearFrame();

  int row = 3;
  int col = 5;

  // Physical display layout:
  //
  // N3  N2  N1
  // N4  N7  N8
  // N9  N5  N6

  if (index == 2) { row = 0; col = 1; }  // N3
  if (index == 1) { row = 0; col = 5; }  // N2
  if (index == 0) { row = 0; col = 9; }  // N1

  if (index == 3) { row = 3; col = 1; }  // N4
  if (index == 6) { row = 3; col = 5; }  // N7
  if (index == 7) { row = 3; col = 9; }  // N8

  if (index == 8) { row = 6; col = 1; }  // N9
  if (index == 4) { row = 6; col = 5; }  // N5
  if (index == 5) { row = 6; col = 9; }  // N6

  setPixelMapped(row, col);
  setPixelMapped(row, col + 1);
  setPixelMapped(row + 1, col);
  setPixelMapped(row + 1, col + 1);

  matrix.renderBitmap(frame, 8, 12);
}

void drawIdle() {
  clearFrame();

  // Four single corner LEDs = idle / no confident light direction detected

  setPixelMapped(0, 0);    // top left
  setPixelMapped(0, 11);   // top right
  setPixelMapped(7, 0);    // bottom left
  setPixelMapped(7, 11);   // bottom right

  matrix.renderBitmap(frame, 8, 12);
}

void loop() {
  float score;

  int rawWinner = classifyLive(score);

  float activity = computeActivityFromRawDelta();

  if (activity < ACTIVITY_THRESHOLD) {
    resetHistoryToCenter();
    drawIdle();

    Serial.print(millis());
    Serial.print(",");

    for (int i = 0; i < NUM_SENSORS; i++) {
      Serial.print(values[i]);
      Serial.print(",");
    }

    Serial.print(activity, 2);
    Serial.println(",NONE,NONE,0.000");

    delay(LIVE_DELAY);
    return;
  }

  int stableWinner = getStableWinner(rawWinner);

  drawDotForSensor(stableWinner);

  Serial.print(millis());
  Serial.print(",");

  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print(values[i]);
    Serial.print(",");
  }

  Serial.print(activity, 2);
  Serial.print(",");

  Serial.print("N");
  Serial.print(rawWinner + 1);
  Serial.print(",");

  Serial.print("N");
  Serial.print(stableWinner + 1);
  Serial.print(",");

  Serial.println(score, 3);

  delay(LIVE_DELAY);
}