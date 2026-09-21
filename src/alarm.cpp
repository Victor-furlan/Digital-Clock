#include "alarm.h"
#include "rtc.h"
#include "buzzer.h"
#include "display.h"
#include <Preferences.h>
#include <Arduino.h>
#include <RTClib.h>

Alarme alarmes[3];

Preferences prefs;

void inicializeAlarms()
{
    prefs.begin("alarmes", true);
    
    for (int i = 0; i < 3; i++)
    {
        alarmes[i].nome = prefs.getString(("a" + String(i) + "nome").c_str(), "");
        alarmes[i].icone = prefs.getString(("a" + String(i) + "icone").c_str(), "");
        alarmes[i].hora = prefs.getInt(("a" + String(i) + "hora").c_str(), 0);
        alarmes[i].minuto = prefs.getInt(("a" + String(i) + "minuto").c_str(), 0);
        alarmes[i].melodia = prefs.getString(("a" + String(i) + "melodia").c_str(), "");
        alarmes[i].ativo = prefs.getBool(("a" + String(i) + "ativo").c_str(), false);
    }
    

    prefs.end();
}

void saveAlarms() {
        prefs.begin("alarmes", false);
    
    for (int i = 0; i < 3; i++)
    {
        prefs.putString(("a" + String(i) + "nome").c_str(), alarmes[i].nome);
        prefs.putString(("a" + String(i) + "icone").c_str(), alarmes[i].icone);
        prefs.putInt(("a" + String(i) + "hora").c_str(), alarmes[i].hora);
        prefs.putInt(("a" + String(i) + "minuto").c_str(), alarmes[i].minuto);
        prefs.putString(("a" + String(i) + "melodia").c_str(), alarmes[i].melodia);
        prefs.putBool(("a" + String(i) + "ativo").c_str(), alarmes[i].ativo);
    }

    prefs.end();
}

void checkAlarms() {
    for (int i = 0; i < 3; i++)
    {
       DateTime agora = getHour();
       if (alarmes[i].ativo && alarmes[i].hora == agora.hour() && alarmes[i].minuto == agora.minute())
       {
            triggerAlarm(alarmes[i]);
       }
       
    }
    
}

void triggerAlarm(Alarme a) {
    handleAlarm(a);
    playRingTone(a.melodia);
}