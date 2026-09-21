#ifndef RTC_H
#define RTC_H

#include <Arduino.h>
#include <RTClib.h>

void inicializeRTC();

void setHour(DateTime hora);

DateTime getHour();

#endif