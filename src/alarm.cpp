#include "alarm.h"
#include "rtc.h"
#include "buzzer.h"
#include "display.h"
#include <Preferences.h>
#include <Arduino.h>
#include <RTClib.h>

Alarme alarmes[3];
Preferences prefs;

static int ultimoMinutoDisparado = -1;
static int ultimaHoraDisparada = -1;

void inicializeAlarms()
{
    prefs.begin("alarmes", true);
    
    for (int i = 0; i < 3; i++)
    {
        alarmes[i].nome = prefs.getString(("a" + String(i) + "nome").c_str(), i == 0 ? "Acordar" : (i == 1 ? "Remédio" : "Academia"));
        alarmes[i].icone = prefs.getString(("a" + String(i) + "icone").c_str(), "💼");
        alarmes[i].hora = prefs.getInt(("a" + String(i) + "hora").c_str(), 7 + i);
        alarmes[i].minuto = prefs.getInt(("a" + String(i) + "minuto").c_str(), 0);
        alarmes[i].melodia = prefs.getString(("a" + String(i) + "melodia").c_str(), "mario");
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
    DateTime agora = getHour();
    
    // Reseta trava de disparo quando o minuto mudar
    if (agora.minute() != ultimoMinutoDisparado || agora.hour() != ultimaHoraDisparada) {
        // pronto para novo disparo
    } else {
        return; // Ja disparou neste minuto
    }

    for (int i = 0; i < 3; i++)
    {
       if (alarmes[i].ativo && alarmes[i].hora == agora.hour() && alarmes[i].minuto == agora.minute())
       {
            ultimoMinutoDisparado = agora.minute();
            ultimaHoraDisparada = agora.hour();
            triggerAlarm(alarmes[i]);
            break;
       }
    }
}

void triggerAlarm(Alarme a) {
    Serial.print("[ALARME] Disparando alarme: ");
    Serial.println(a.nome);
    handleAlarm(a);
    playRingTone(a.melodia);
}