#ifndef APP_WEBSERVER_H
#define APP_WEBSERVER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "alarm.h"

extern Alarme alarmes[3];

void inicializeWebServer();

void handleWebServer();

#endif
