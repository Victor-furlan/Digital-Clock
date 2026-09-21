#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

extern Alarme alarmes[3];

void inicializeWebServer();

void handleWebServer();

#endif