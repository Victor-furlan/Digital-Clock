#include <Arduino.h>
#include "config.h"
#include "rtc.h"
#include "wifi_ntp.h"
#include "buzzer.h"
#include "touch.h"
#include "alarm.h"
#include "display.h"
#include "app_webserver.h"

extern Alarme alarmes[3];

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n=========================================");
  Serial.println("  Iniciando Relogio Despertador Smart IoT  ");
  Serial.println("=========================================");

  initializeTTP();
  inicializeRTC();
  inicializeAlarms();

  handleSplash();
  connectWifi();
  syncNTP();

  handleWelcome();
  delay(3000);

  if (WiFi.status() == WL_CONNECTED) {
    handleQrCode(WiFi.localIP().toString());
  } else {
    handleQrCode("Sem Wi-Fi");
  }
  delay(10000);

  inicializeWebServer();
  Serial.println("[SYSTEM] Setup concluido. Entrando no loop principal.");
}

void loop() {
  handleWebServer();
  checkAlarms();
  handleIndex(getHour(), alarmes);
  delay(1000);
}