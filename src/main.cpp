#include <Arduino.h>
#include "config.h"
#include "rtc.h"
#include "wifi_ntp.h"
#include "buzzer.h"
#include "touch.h"
#include "alarm.h"
#include "display.h"
#include "webserver.h"

extern Alarme alarmes[3];

void setup() {
  Serial.begin(115200);
  inicializeRTC();
  handleSplash();
  connectWifi();
  syncNTP();
  handleWelcome();
  delay(4000);
  handleQrCode(WiFi.localIP().toString());
  inicializeAlarms();
  inicializeWebServer();
  
}

void loop() {
  handleWebServer();
  checkAlarms();
  handleIndex(getHour(), alarmes);
  delay(1000);
}