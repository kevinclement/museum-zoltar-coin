#include <Arduino.h>
#include "version.h"
#include "coin.h"
#include "led.h"
#include "lights.h"

Coin coin;
LED led;
Lights lights;

// functions
void readAnySerialMessage();
void fake();
void real();
void status();
void solved();
void(* resetFunc) (void) = 0;

// track coin count
unsigned int coin_count = 0;
unsigned int donation_count = 0;

bool _solved = false;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);
  
  Serial.println("Museum zoltar coin detector by kevinc...");
  Serial.println(getFullVersion("museum-zoltar-coin"));

  coin.setup();
  led.setup();
  lights.setup();

  status();

  attachInterrupt(digitalPinToInterrupt(DONATE_PIN), fake, RISING);
  attachInterrupt(digitalPinToInterrupt(TOKEN_PIN), real, RISING);
}

void loop() {
  readAnySerialMessage();

  coin.handle();
  led.handle();
  lights.handle();

  if (coin.count != coin_count) {
    Serial.print("token detected (");
    Serial.print(coin.count);
    Serial.println(").  changing led.");

    if (coin_count < 3) {
      led.state[coin_count] = true;
    }
    coin_count = coin.count;

    if (coin_count == 3) {
      solved();
    }

    status();
  }
  
  if (coin.donations != donation_count) {
    Serial.println("donation detected.");
    donation_count = coin.donations;
    status();
  }
}

void solved() {
  _solved = true;
}

void status() {
  char cMsg[254];
  sprintf(cMsg, 
    "status="
      "version:%s,"
      "gitDate:%s,"
      "buildDate:%s,"
      "solved:%s,"
      "coins:%d,"
      "donations:%d"
    , GIT_HASH,
      GIT_DATE,
      DATE_NOW,
      _solved ? "true" : "false",
      coin_count,
      donation_count);

  Serial.println(cMsg);
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
    // blink_lights = !blink_lights;
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

// I dont want to figure out static callers and such to get this working
// in class world with attachinterupt, so just calling instance
void fake() {
  coin.fake();
}
void real() {
  coin.real();
}
