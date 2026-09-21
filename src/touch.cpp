#include "touch.h"
#include "config.h"
#include <Arduino.h>

void initializeTTP() {
    pinMode(TTP, INPUT);
}

int identifyTouchType() {
    long inicio = millis();
    while (digitalRead(TTP) == HIGH){}
    long tempo = millis() - inicio;
    
    if (tempo <= 1000 && tempo > 0)
    {
        return 1;
    }else if (tempo > 1000)
    {
        return 2;
    }else {
        return 0;
    }
    
    
}