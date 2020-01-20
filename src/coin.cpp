#include "Arduino.h"
#include "coin.h"

unsigned int REAL_THRESHOLD = 500;
unsigned int DONATION_THRESHOLD = 1000;
unsigned int LAST_COIN_THRESHOLD = 1500;

unsigned long fake_coin_timestamp = 0;
unsigned long real_coin_timestamp = 0;
unsigned long last_coin_detected = 0;

bool triggered_token = false;

Coin::Coin()
{  
}

void Coin::setup() {
  pinMode(DONATE_PIN, INPUT_PULLUP);
  pinMode(TOKEN_PIN, INPUT_PULLUP);
}

void Coin::handle() {
  if (real_coin_timestamp > 0 && millis() - real_coin_timestamp > REAL_THRESHOLD) {
    token();
  }

  if (fake_coin_timestamp > 0 && millis() - fake_coin_timestamp > DONATION_THRESHOLD) {
    donations++;
    fake_coin_timestamp = 0;

    //Serial.print("### DONATE: ");
    //Serial.println(donations);
  }
}

void Coin::fake() {
  // Serial.println("** fake");
  fake_coin_timestamp = millis();

  // if we see a fake, and haven't timed out the real coin
  // then its inserted really fast, just trigger a real then
  if (real_coin_timestamp > 0 && !triggered_token) {
    Serial.println("INFO: noticed a fake coin but haven't triggered the real one yet, must be fast.");
    token();
    triggered_token = true;
  }
}

void Coin::real() {
  // need to reset these in case we ignore further triggers, we don't want to trigger a donation
  fake_coin_timestamp = 0;
  triggered_token = false;

  if (last_coin_detected > 0) {
    if (millis() - last_coin_detected < LAST_COIN_THRESHOLD) {
      Serial.println("INFO: detected real coin too soon.  Ignoring that trigger.");
      return;
    }
  }

  real_coin_timestamp = millis();
}

void Coin::increment() {
  count++;
}

void Coin::decrement() {
  if (count > 0) {
    count--;
  } else {
    Serial.println("WARN: count was at zero.  not decrementing further.");
  }
}

void Coin::solve() {
  count = 3;
}

void Coin::token() {
  count++;
  real_coin_timestamp = 0;
  last_coin_detected = millis();

  // Serial.print("### TOKEN: ");
  // Serial.println(count);
}