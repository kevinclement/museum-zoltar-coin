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
void coinChange();
void mock();
void(* resetFunc) (void) = 0;

// track coin count
unsigned int coin_count = 0;
unsigned int donation_count = 0;

unsigned long _solved_at = 0;
bool _mock = false;

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
    coinChange();
    status();
  }
  
  if (coin.donations != donation_count) {
    Serial.println("donation detected.");
    donation_count = coin.donations;
    status();
  }

  mock();
}

void solved() {
  _solved_at = millis();
  lights.trigger();
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
      _solved_at > 0 ? "true" : "false",
      coin_count,
      donation_count);

  Serial.println(cMsg);
}

void coinChange() {
  Serial.print("token detected (");
  Serial.print(coin.count);
  Serial.println(").");

  coin_count = coin.count;

  for (unsigned int i=0;i<3;i++) {
    led.state[i] = i < coin_count;
  }

  if (coin_count == 3) {
    solved();
  }
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
    Serial.println("manually triggering lights...");
    lights.trigger();
  }
  else if (msg == "increment" || msg == "i") {
    Serial.println("manually incrementing coin count...");
    coin.increment();
  }
  else if (msg == "decrement" || msg == "d") {
    Serial.println("manually decrementing coin count...");
    coin.decrement();
  }
  else if (msg == "solve" || msg == "v") {
    coin.solve();
  }
  else if (msg == "mock" || msg == "m") {
    _mock = !_mock;
  }
  else if (msg == "reset" || msg == "reboot" || msg == "r") {
    resetFunc();
  } else {
    Serial.print("unknown command: ");
    Serial.println(msg);
  }
}

void mock() {
  static int mocktime = millis();
  if (_mock && mocktime > 0 && millis() - mocktime > 3000) {
    coin.real();

    if (coin_count < 2) {
      mocktime = millis();
    } else {
      mocktime = 0;
    }
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
