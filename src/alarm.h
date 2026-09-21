#ifndef ALARM_H
#define ALARM_H
#include <Arduino.h>

struct Alarme
{
    String nome;
    String icone;
    int hora;
    int minuto;
    String melodia;
    bool ativo;
};

    void inicializeAlarms();

    void saveAlarms();

    void checkAlarms();
    
    void triggerAlarm(Alarme a);
#endif