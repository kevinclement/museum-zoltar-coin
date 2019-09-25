#include <Arduino.h>
#include "version.h"
#include "coin.h"
#include "led.h"
#include "lights.h"

Coin coin;
LED led;
Lights lights;

void readAnySerialMessage();

#define DONATE_PIN PD2
#define TOKEN_PIN PD3

unsigned int REAL_THRESHOLD = 500;
unsigned int DONATION_THRESHOLD = 1000;

#define LED1_PIN 12
#define LED2_PIN 11
#define LED3_PIN 10

#define MAIN_LIGHT_PIN PD7

volatile int fakeCount = 0;
volatile int realCount = 0;

unsigned long fake_coin_timestamp = 0;
unsigned long real_coin_timestamp = 0;

bool led_state[3] = {false};

void(* resetFunc) (void) = 0;

bool triggeredRealFromFake = false;
void realCoinInserted() {
  if (realCount < 3) {
    led_state[realCount] = true;
  }
  realCount++;
    
  Serial.print("### TOKEN: ");
  Serial.println(realCount);
  real_coin_timestamp = 0;
}

void fakeCoin() {
  Serial.println("** fake");
  fake_coin_timestamp = millis();

  // if we see a fake, and haven't timed out the real coin
  // then its inserted really fast, just trigger a real then
  if (real_coin_timestamp > 0 && !triggeredRealFromFake) {
    realCoinInserted();
    triggeredRealFromFake = true;
  }
}

void realCoin() {
  Serial.println("** real");
  real_coin_timestamp = millis();
  Serial.print("    ");
  Serial.println(real_coin_timestamp);
  fake_coin_timestamp = 0;
  triggeredRealFromFake = false;
}

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);
  
  Serial.println("Museum zoltar coin detector by kevinc...");
  Serial.println(getFullVersion("museum-zoltar-coin"));

  coin.setup();
  led.setup();
  lights.setup();

  pinMode(DONATE_PIN, INPUT_PULLUP);
  pinMode(TOKEN_PIN, INPUT_PULLUP);
  pinMode(MAIN_LIGHT_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(DONATE_PIN), fakeCoin, RISING);
  attachInterrupt(digitalPinToInterrupt(TOKEN_PIN), realCoin, RISING);
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

void loop() {
  readAnySerialMessage();

  coin.handle();
  led.handle();
  lights.handle();

// back to back trace:
// ** fake
// ** real
//     29677
// ** real
//     29677
// ** real
//     29678
// ** real
//     29679
// ** real
//     29679
// ** real
//     29679
// ** fake
// ** real
//     30099
// ** real
//     30115
// ### TOKEN: 1

  analogWrite(10, led_state[0] ? 255 : 0);
  analogWrite(11, led_state[1] ? 255 : 0);
  analogWrite(12, led_state[2] ? 255 : 0);

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

  if (real_coin_timestamp > 0 && millis() - real_coin_timestamp > REAL_THRESHOLD) {
    realCoinInserted();
  }

  //    Serial.print("*** DONATE: ");
    // Serial.println(donateCount);
}

void readAnySerialMessage() {
  if (!Serial.available()) {
    return;
  }

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