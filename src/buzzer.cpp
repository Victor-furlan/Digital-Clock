#include "buzzer.h"
#include "config.h"
#include "touch.h"
#include <Arduino.h>

static const char* RTTTL_MARIO = "Mario:d=4,o=5,b=100:16e6,16e6,16p,16e6,16p,16c6,16e6,8g6,8p,8g";
static const char* RTTTL_STARWARS = "StarWars:d=4,o=5,b=45:8e,8e,8e,16c,16p,8g,8e,16c,16p,8g,4e";
static const char* RTTTL_HARRYPOTTER = "HarryPotter:d=4,o=5,b=140:4b4,8e,16g,8f#,4e,8b,4a.,4f#.";

void playRingTone(String rttl) {
    String song = rttl;
    song.trim();
    song.toLowerCase();

    if (song == "mario" || song.indexOf("mario") != -1) {
        song = RTTTL_MARIO;
    } else if (song == "starwars" || song.indexOf("starwars") != -1) {
        song = RTTTL_STARWARS;
    } else if (song == "harrypotter" || song.indexOf("harrypotter") != -1) {
        song = RTTTL_HARRYPOTTER;
    } else if (song.indexOf(':') == -1) {
        // Fallback padrao se string nao contem formato RTTTL
        song = RTTTL_MARIO;
    }

    // Separa as 3 partes do RTTTL
    int pos1 = song.indexOf(':');
    int pos2 = song.indexOf(':', pos1 + 1);

    if (pos1 == -1 || pos2 == -1) return;

    String config = song.substring(pos1 + 1, pos2);
    String notas  = song.substring(pos2 + 1);

    // Extrai configurações padrão
    int bpmPos = config.indexOf("b=");
    int durPos = config.indexOf("d=");
    int octPos = config.indexOf("o=");

    int bpm    = (bpmPos != -1) ? config.substring(bpmPos + 2, config.indexOf(',', bpmPos)).toInt() : 100;
    int durPad = (durPos != -1) ? config.substring(durPos + 2, config.indexOf(',', durPos)).toInt() : 4;
    int octPad = (octPos != -1) ? config.substring(octPos + 2, config.indexOf(',', octPos)).toInt() : 5;

    if (bpm <= 0) bpm = 100;
    if (durPad <= 0) durPad = 4;

    // Duração de um tempo em ms
    int tempoDur = 60000 / bpm;

    // Frequências das notas (índice: 0=pausa, 1=C, 2=D, 3=E, 4=F, 5=G, 6=A, 7=B)
    int freqs[] = {0, 262, 294, 330, 349, 392, 440, 494};

    // Processa cada nota
    int i = 0;
    while (i < notas.length()) {
        if (isTouched()) {
            noTone(BUZZER);
            break;
        }

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
        if (dur <= 0) dur = 4;
        int ms = tempoDur * 4 / dur;
        if (ponto) ms = ms * 1.5;

        // Toca
        if (freq > 0) {
            tone(BUZZER, freq, ms * 0.9);
        }
        
        // Espera nota respeitando interrupcao por toque
        int elapsed = 0;
        while (elapsed < ms) {
            if (isTouched()) {
                noTone(BUZZER);
                return;
            }
            delay(10);
            elapsed += 10;
        }
        
        noTone(BUZZER);

        // Pula vírgula
        if (i < notas.length() && notas[i] == ',') i++;
    }
}