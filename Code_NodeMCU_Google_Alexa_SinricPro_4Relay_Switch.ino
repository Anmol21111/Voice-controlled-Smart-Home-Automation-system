/**********************************************************************************
 * NodeMCU (ESP8266) - 3 Relays control with SinricPro
 * - D0 -> Relay 1  (GPIO16)  Bulb 1
 * - D1 -> Relay 2  (GPIO5)   Bulb 2
 * - D2 -> Relay 3  (GPIO4)   Fan
 **********************************************************************************/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "SinricPro.h"
#include "SinricProSwitch.h"

/* ---------- CONFIG - UPDATE ---------- */
#define WIFI_SSID   "internet"
#define WIFI_PASS   "123456789"

#define APP_KEY     "967d7f43-1ba6-4470-aa52-84b7b43ec720"
#define APP_SECRET  "c9309612-d373-4777-8699-a02774f4f9e0-b33412eb-6033-4b94-9ea1-c7bfca26b392"

// SinricPro device IDs
#define DEVICE_ID_1 "690e4bfdc3bac56fb15a758a"
#define DEVICE_ID_2 "690e4bc9fb7ad92678891580"
#define DEVICE_ID_3 "69107ebbfb7ad9267889f3f1"
/* -------------------------------------- */

/* NodeMCU pin mapping
   D0 -> GPIO16
   D1 -> GPIO5
   D2 -> GPIO4
*/
const uint8_t RELAY_PIN_1 = 16;   // Bulb 1
const uint8_t RELAY_PIN_2 = 5;    // Bulb 2
const uint8_t RELAY_PIN_3 = 4;    // Fan

#define BAUD_RATE 115200

void setupRelays() {
  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT);
  pinMode(RELAY_PIN_3, OUTPUT);

  // Active LOW
  digitalWrite(RELAY_PIN_1, HIGH);
  digitalWrite(RELAY_PIN_2, HIGH);
  digitalWrite(RELAY_PIN_3, HIGH);
}

bool onPowerState(const String &deviceId, bool state) {   
  uint8_t relayPin = 255;

  if (deviceId == DEVICE_ID_1) relayPin = RELAY_PIN_1;
  else if (deviceId == DEVICE_ID_2) relayPin = RELAY_PIN_2;
  else if (deviceId == DEVICE_ID_3) relayPin = RELAY_PIN_3;
  else return false;

  digitalWrite(relayPin, state ? LOW : HIGH);   // Active LOW
  return true;
}

void setupWiFi() {
  Serial.printf("\n[WiFi] Connecting to %s", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.printf("\n[WiFi] Connected! IP: %s\n", WiFi.localIP().toString().c_str());
}

void setupSinricPro() {
  SinricProSwitch &bulb1 = SinricPro[DEVICE_ID_1];
  bulb1.onPowerState(onPowerState);

  SinricProSwitch &bulb2 = SinricPro[DEVICE_ID_2];
  bulb2.onPowerState(onPowerState);

  SinricProSwitch &fan = SinricPro[DEVICE_ID_3];
  fan.onPowerState(onPowerState);

  SinricPro.begin(APP_KEY, APP_SECRET);
  SinricPro.restoreDeviceStates(true);
}

void setup() {
  Serial.begin(BAUD_RATE);
  Serial.println("\n=== NodeMCU SinricPro - 3 Relays (D0, D1, D2) ===");

  setupRelays();
  setupWiFi();
  setupSinricPro();
}

void loop() {
  SinricPro.handle();
}