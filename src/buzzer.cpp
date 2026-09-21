#include "buzzer.h"
#include "config.h"
#include <Arduino.h>

void playRingTone(String rttl) {
    // Separa as 3 partes do RTTTL
    int pos1 = rttl.indexOf(':');
    int pos2 = rttl.indexOf(':', pos1 + 1);

    String config = rttl.substring(pos1 + 1, pos2);
    String notas  = rttl.substring(pos2 + 1);

    // Extrai configurações padrão
    int bpmPos = config.indexOf("b=");
    int durPos = config.indexOf("d=");
    int octPos = config.indexOf("o=");

    int bpm      = config.substring(bpmPos + 2, config.indexOf(',', bpmPos)).toInt();
    int durPad   = config.substring(durPos + 2, config.indexOf(',', durPos)).toInt();
    int octPad   = config.substring(octPos + 2, config.indexOf(',', octPos)).toInt();

    // Duração de um tempo em ms
    int tempoDur = 60000 / bpm;

    // Frequências das notas (índice: 0=pausa, 1=C, 2=D, 3=E, 4=F, 5=G, 6=A, 7=B)
    int freqs[] = {0, 262, 294, 330, 349, 392, 440, 494};

    // Processa cada nota
    int i = 0;
    while (i < notas.length()) {
        // Duração da nota (opcional no início)
        int dur = durPad;
        if (isDigit(notas[i])) {
            dur = 0;
            while (i < notas.length() && isDigit(notas[i])) {
                dur = dur * 10 + (notas[i] - '0');
                i++;
            }
        }

        // Nota
        int freq = 0;
        char nota = tolower(notas[i]);
        i++;
        switch (nota) {
            case 'c': freq = freqs[1]; break;
            case 'd': freq = freqs[2]; break;
            case 'e': freq = freqs[3]; break;
            case 'f': freq = freqs[4]; break;
            case 'g': freq = freqs[5]; break;
            case 'a': freq = freqs[6]; break;
            case 'b': freq = freqs[7]; break;
            case 'p': freq = 0;        break;
        }

        // Sustenido (#)
        if (i < notas.length() && notas[i] == '#') {
            freq = freq * 1.059;
            i++;
        }

        // Ponto (aumenta duração em 50%)
        bool ponto = false;
        if (i < notas.length() && notas[i] == '.') {
            ponto = true;
            i++;
        }

        // Oitava
        int oitava = octPad;
        if (i < notas.length() && isDigit(notas[i])) {
            oitava = notas[i] - '0';
            i++;
        }

        // Ajusta frequência pela oitava (oitava 4 = base)
        for (int o = 4; o < oitava; o++) freq *= 2;
        for (int o = oitava; o < 4; o++) freq /= 2;

        // Calcula duração em ms
        int ms = tempoDur * 4 / dur;
        if (ponto) ms = ms * 1.5;

        // Toca
        if (freq > 0) {
            tone(BUZZER, freq, ms * 0.9);
        }
        delay(ms);
        noTone(BUZZER);

        // Pula vírgula
        if (i < notas.length() && notas[i] == ',') i++;
    }
}