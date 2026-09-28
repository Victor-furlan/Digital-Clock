#include "buzzer.h"
#include "config.h"
#include "touch.h"
#include <Arduino.h>

static const char* RTTTL_MARIO        = "Mario:d=4,o=5,b=200:16e6,16e6,32p,8e6,16c6,8e6,8g6,8p,8g5,8p,8c6,16p,8g5,16p,8e5,16p,8a5,8b5,16a#5,8a5,16g5,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b5";
static const char* RTTTL_STARWARS     = "StarWars:d=4,o=5,b=112:a4,a4,a4,f4,16c5,a4,f4,16c5,2a4,e5,e5,e5,f5,16c5,a#4,f4,16c5,2a4";
static const char* RTTTL_HARRYPOTTER  = "HarryPotter:d=8,o=5,b=140:b4,e5,g5,f#5,e5,2b5,a5,2f#5,2e5,2b5,e5,g5,f#5,d5,2f5,4e5";
static const char* RTTTL_SAMSUNG      = "Samsung:d=8,o=5,b=125:c6,d6,e6,f6,e6,f6,g6,p,e6,f6,g6,a6,g6,a6,b6,p,4g6,4a6,4g6,4e6";
static const char* RTTTL_TETRIS       = "Tetris:d=4,o=5,b=160:e6,8b5,8c6,d6,8c6,8b5,a5,8a5,8c6,e6,8d6,8c6,b5,8c6,d6,e6,c6,a5,a5";
static const char* RTTTL_BATMAN       = "Batman:o=5,d=8,b=180:d,d,c#,c#,c,c,c#,c#,d,d,c#,c#,c,c,c#,c#,d,d#,c,c#,c,c,c#,c#,f,p,4f";
static const char* RTTTL_LETITBE      = "LetItBe:o=5,d=8,b=100:16e6,d6,4c6,16e6,g6,a6,g6.,16g6,g6,e6,16d6,c6,16a,g,4e6.,4p,e6,16e6,f6.,e6,e6,d6,16p,16e6,16d6,d6,2c6..";
static const char* RTTTL_MACARENA     = "Macarena:o=5,d=8,b=180:f,f,f,4f,f,f,f,f,f,f,f,a,c,c,4f,f,f,4f,f,f,f,f,f,f,d,c,4p,4f,f,f,4f,f,f,f,f,f,f,f,a,4p,2c6.,4a,c6,a,f,4p,2p";
static const char* RTTTL_SMURFS       = "Smurfs:o=5,d=4,b=200:2c6,f6.,8c6,d6,a#,2g,c6.,8a,f,a,2g,p,16g,16a,16a#,16b,2c6";
static const char* RTTTL_SPIDERMAN    = "Spiderman:o=6,d=4,b=200:c,8d#,g.,p,f#,8d#,c.,p,c,8d#,g,8g#,g,f#,8d#,c.,p,f,8g#,c7.,p,a#,8g#,f.,p,c,8d#,g.,p,f#,8d#,c,p,8g#,2g,p,8f#,f#,8d#,f,8d#,2c";
static const char* RTTTL_TAKEONME     = "TakeOnMe:o=5,d=8,b=160:f#,f#,f#,d,p,b4,p,e,p,e,p,e,g#,g#,a,b,a,a,a,e,p,d,p,f#,p,f#,p,f#,e,e,f#,e";
static const char* RTTTL_TITANIC      = "Titanic:o=6,d=8,b=120:c,d,2e.,d,c,d,g,2g,f,e,4c,2a5,g5,f5,16d5,16e5,2d5,p,c,d,2e.,d,c,d,g,2g,e,g,2a,2g,16d,16e,2d.";

void playRingTone(String rttl) {
    String song = rttl;
    song.trim();
    song.toLowerCase();

    if (song.indexOf("mario") != -1)          { song = RTTTL_MARIO; }
    else if (song.indexOf("starwars") != -1)  { song = RTTTL_STARWARS; }
    else if (song.indexOf("harrypotter") != -1){ song = RTTTL_HARRYPOTTER; }
    else if (song.indexOf("samsung") != -1)   { song = RTTTL_SAMSUNG; }
    else if (song.indexOf("tetris") != -1)    { song = RTTTL_TETRIS; }
    else if (song.indexOf("batman") != -1)    { song = RTTTL_BATMAN; }
    else if (song.indexOf("letitbe") != -1)   { song = RTTTL_LETITBE; }
    else if (song.indexOf("macarena") != -1)  { song = RTTTL_MACARENA; }
    else if (song.indexOf("smurfs") != -1)    { song = RTTTL_SMURFS; }
    else if (song.indexOf("spiderman") != -1) { song = RTTTL_SPIDERMAN; }
    else if (song.indexOf("takeonme") != -1)  { song = RTTTL_TAKEONME; }
    else if (song.indexOf("titanic") != -1)   { song = RTTTL_TITANIC; }
    else if (song.indexOf(':') == -1)          { song = RTTTL_SAMSUNG; }

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

    int tempoDur = 60000 / bpm;

    int freqs[] = {0, 262, 294, 330, 349, 392, 440, 494};

    int i = 0;
    while (i < notas.length()) {
        if (isTouched()) { noTone(BUZZER); break; }

        int dur = durPad;
        if (isDigit(notas[i])) {
            dur = 0;
            while (i < notas.length() && isDigit(notas[i])) {
                dur = dur * 10 + (notas[i] - '0');
                i++;
            }
        }

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

        if (i < notas.length() && notas[i] == '#') { freq = freq * 1.059; i++; }

        bool ponto = false;
        if (i < notas.length() && notas[i] == '.') { ponto = true; i++; }

        int oitava = octPad;
        if (i < notas.length() && isDigit(notas[i])) { oitava = notas[i] - '0'; i++; }

        // Ajusta frequência pela oitava
        if (oitava > octPad) {
            for (int o = octPad; o < oitava; o++) freq *= 2;
        } else if (oitava < octPad) {
            for (int o = oitava; o < octPad; o++) freq /= 2;
        }

        if (dur <= 0) dur = 4;
        int ms = tempoDur * 4 / dur;
        if (ponto) ms = ms * 1.5;

        if (freq > 0) tone(BUZZER, freq, ms * 0.9);

        int elapsed = 0;
        while (elapsed < ms) {
            if (isTouched()) { noTone(BUZZER); return; }
            delay(10);
            elapsed += 10;
        }

        noTone(BUZZER);
        if (i < notas.length() && notas[i] == ',') i++;
    }
}