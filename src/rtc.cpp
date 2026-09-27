#include "rtc.h"
#include "config.h"
#include <Arduino.h>
#include <Wire.h>

RTC_DS3231 rtc;
static bool rtcOK = false;

void inicializeRTC(){
    Wire.begin(RTC_SDA, RTC_SCL);
    if (!rtc.begin())
    {
        Serial.println("[RTC] DS3231 nao encontrado via I2C!");
        rtcOK = false;
    } else {
        rtcOK = true;
        Serial.println("[RTC] DS3231 inicializado com sucesso.");
        DateTime now = rtc.now();
        if (rtc.lostPower() || now.year() < 2024) {
            Serial.println("[RTC] Ajustando hora padrao para data de compilacao...");
            rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        }
    }
}

void setHour(DateTime hora) {
    if (rtcOK) {
        rtc.adjust(hora);
    }
}

DateTime getHour() {
    if (rtcOK) {
        DateTime now = rtc.now();
        if (now.year() >= 2024 && now.year() <= 2099) {
            return now;
        }
    }
    // Fallback para relogio interno do ESP32 (sys time)
    time_t t = time(NULL);
    if (t > 1000000000UL) {
        return DateTime((uint32_t)t);
    }
    // Se nem RTC nem NTP estiverem prontos, retorna 12:00
    return DateTime(2026, 9, 27, 12, 0, 0);
}