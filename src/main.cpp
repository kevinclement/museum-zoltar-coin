#include <Arduino.h>
#include "version.h"

#define DONATE_PIN PD2
#define TOKEN_PIN PD3
int DONATION_THRESHOLD = 3000;

#define LED1_PIN PD12
#define LED2_PIN PD11
#define MAIN_LIGHT_PIN PD7


volatile int donateCount = 0;
volatile int tokenCount = 0;

unsigned long donation_detected_timestamp = 0;

void(* resetFunc) (void) = 0;

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
  Serial.setTimeout(10);
  Serial.println("Museum zoltar coin detector by kevinc...");
  Serial.println(getFullVersion("museum-zoltar-coin"));

  pinMode(DONATE_PIN, INPUT_PULLUP);
  pinMode(TOKEN_PIN, INPUT_PULLUP);
  pinMode(MAIN_LIGHT_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(DONATE_PIN), coinDonated, RISING);
  attachInterrupt(digitalPinToInterrupt(TOKEN_PIN), tokenInserted, RISING);
}

// status=version:vb7e8fa7-dirty,buildDate:2009-11-10 11:09,gitDate:2019-08-06 14:51:47

unsigned long light_time = millis();
bool light_on = false;

#define TIMES 7
void flash() {
  for (int i=0; i< random(TIMES); i++)
  {
    analogWrite(MAIN_LIGHT_PIN, 255);
    delay(62);
    analogWrite(MAIN_LIGHT_PIN, 0);
    delay(10);
  }
}

unsigned long lastTime = 0;
int waitTime = 1500;
bool blink_lights = false;

void readAnySerialMessage() {
  if (!Serial.available()) {
    return;
  }

  // read and handle message from serial
  String msg = Serial.readStringUntil('\n');
  Serial.print("got '");
  Serial.print(msg);
  Serial.println("' command");

  if (msg == "lights" || msg == "l") {
    Serial.println("toggling blinking lights...");
    blink_lights = !blink_lights;
  }
  else if (msg == "solve" || msg == "v") {
  }
  else if (msg == "reset" || msg == "reboot" || msg == "r") {
    resetFunc();
  } else {
    Serial.print("unknown command: ");
    Serial.println(msg);
  }
}

void loop() {
  readAnySerialMessage();

  if (blink_lights) {
    if (millis() - lastTime > waitTime)  // time for a new flash
    {
        // adjust timing params
        flash();
        lastTime = millis();
    }
  } else {
    analogWrite(MAIN_LIGHT_PIN, 0);
  }

  // int inState = digitalRead(DONATE_PIN);
  // Serial.println(inState);
  // delay(100);
  if (donation_detected_timestamp > DONATION_THRESHOLD) {
    donateCount++;
    donation_detected_timestamp = 0;
    Serial.print("*** DONATE: ");
    Serial.println(donateCount);
  }
}