#ifndef DISPLAY_H
#define DISPLAY_H
#include <Arduino.h>
#include "alarm.h"
#include <RTClib.h>
#include "rtc.h"

void handleSplash();

void handleWelcome();

void handleQrCode(String ip);

void handleIndex(DateTime hora, Alarme alarmes[]);

void handleAlarm(Alarme alarme);

#endif