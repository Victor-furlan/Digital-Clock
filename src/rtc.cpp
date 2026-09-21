#include "rtc.h"
#include "config.h"
#include <Arduino.h>
#include <Wire.h>

RTC_DS3231 rtc;

void inicializeRTC(){
    Wire.begin(RTC_SDA, RTC_SCL);
    if (!rtc.begin())
    {
        Serial.println("RTC não encontrado");
        while(1);
    }
}

void setHour(DateTime hora) {
    rtc.adjust(hora);
}

DateTime getHour() {
   return rtc.now();
}