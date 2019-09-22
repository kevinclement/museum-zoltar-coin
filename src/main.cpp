#include <Arduino.h>
#include "version.h"

#define DONATE_PIN PD2
int TOKEN_PIN = 35;
int DONATION_THRESHOLD = 3000;

volatile int donateCount = 0;
volatile int tokenCount = 0;

unsigned long donation_detected_timestamp = 0;

void coinDonated() {
  donation_detected_timestamp = millis();
}

void tokenInserted() {
  tokenCount++;
  donation_detected_timestamp = 0;
  Serial.print("### TOKEN: ");
  Serial.println(tokenCount);
}

void setup() {
  Serial.begin(9600);  
  Serial.println("Museum zoltar coin detector by kevinc...");
  Serial.println(getFullVersion("museum-zoltar-coin"));

  pinMode(DONATE_PIN, INPUT_PULLUP);
  pinMode(TOKEN_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(DONATE_PIN), coinDonated, RISING);
  //attachInterrupt(digitalPinToInterrupt(TOKEN_PIN), tokenInserted, RISING);
}

// status=version:vb7e8fa7-dirty,buildDate:2009-11-10 11:09,gitDate:2019-08-06 14:51:47

void loop() {
  // int inState = digitalRead(DONATE_PIN);
  // Serial.println(inState);
  // delay(100);
  if (donation_detected_timestamp > DONATION_THRESHOLD) {
    donateCount++;
    donation_detected_timestamp = 0;
    Serial.print("*** DONATE: ");
    Serial.println(donateCount);
    donateReported = true;
  }
}