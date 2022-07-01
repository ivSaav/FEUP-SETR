#include "include/measures.h"
#include <Arduino.h>


static int timestampIndex = 0;
static unsigned long timestamps[MEASURES];


void addTimestamp() {
  timestamps[timestampIndex] = micros();
  timestampIndex++;
}

unsigned long int* getTimestampArray() {
  return timestamps;
}

bool isDoneMeasureing() {
  return timestampIndex >= MEASURES;
}

void printTimestamps() {
  Serial.println(F("Timestamps: "));
  for (int i = 0; i < MEASURES; i++) {
    Serial.println(timestamps[i]);
    delay(100);
  }
}

void printCurrentTimestampIndex() {
  Serial.print(F("Current timestamp index: "));
  Serial.println(timestampIndex);
}