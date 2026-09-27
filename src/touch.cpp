#include "touch.h"
#include "config.h"
#include <Arduino.h>

void initializeTTP() {
    pinMode(TTP, INPUT_PULLDOWN);
}

bool isTouched() {
    return digitalRead(TTP) == HIGH;
}

int identifyTouchType() {
    if (digitalRead(TTP) == LOW) return 0;
    
    unsigned long inicio = millis();
    while (digitalRead(TTP) == HIGH) {
        if (millis() - inicio > 3000) break; // timeout maximo de 3s
        delay(10);
    }
    unsigned long tempo = millis() - inicio;
    
    if (tempo > 0 && tempo <= 1000)
    {
        return 1; // Toque curto
    } else if (tempo > 1000)
    {
        return 2; // Toque longo
    } else {
        return 0;
    }
}