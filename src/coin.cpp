#include "Arduino.h"
#include "coin.h"

unsigned int REAL_THRESHOLD = 500;
unsigned int DONATION_THRESHOLD = 1000;

unsigned long fake_coin_timestamp = 0;
unsigned long real_coin_timestamp = 0;

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
    Serial.print("*** DONATE: ");
    Serial.println(donations);
  }
}

void Coin::fake() {
  Serial.println("** fake");
  fake_coin_timestamp = millis();

  // if we see a fake, and haven't timed out the real coin
  // then its inserted really fast, just trigger a real then
  if (real_coin_timestamp > 0 && !triggered_token) {
     token();
     triggered_token = true;
  }
}

void Coin::real() {
  Serial.println("** real");
  real_coin_timestamp = millis();
  Serial.print("    ");
  Serial.println(real_coin_timestamp);
  fake_coin_timestamp = 0;
  triggered_token = false;
}

void Coin::token() {
  count++;
  Serial.print("### TOKEN: ");
  Serial.println(count);
  real_coin_timestamp = 0;
}


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