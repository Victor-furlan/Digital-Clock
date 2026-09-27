#include "config.h"
#include "wifi_ntp.h"
#include "rtc.h"
#include <RTClib.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <sys/time.h>

WiFiUDP udp;
NTPClient ntp(udp, "pool.ntp.org", HORARIO);

void connectWifi() {
    Serial.print("[WiFi] Conectando a ");
    Serial.println(WIFI_SSID);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, PASSWORD);
    
    int tentativas = 0;
    while (WiFi.status() != WL_CONNECTED && tentativas < 30)
    {
        delay(500);
        Serial.print(".");
        tentativas++;
    }
    Serial.println();
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.print("[WiFi] Conectado! IP: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("[WiFi] Falha ao conectar no Wi-Fi!");
    }
}

void syncNTP() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("[NTP] WiFi nao conectado. Pulando sincronizacao.");
        return;
    }
    
    Serial.println("[NTP] Sincronizando horario...");
    ntp.begin();
    
    bool ok = false;
    for (int i = 0; i < 10; i++) {
        if (ntp.forceUpdate()) {
            ok = true;
            break;
        }
        delay(500);
    }

    unsigned long epoch = ntp.getEpochTime();
    if (ok && epoch > 1000000000UL) {
        // Atualiza relogio interno do ESP32 (sys time)
        struct timeval tv = { (time_t)epoch, 0 };
        settimeofday(&tv, NULL);

        DateTime agora = DateTime(epoch);
        setHour(agora);
        Serial.print("[NTP] Hora sincronizada com sucesso: ");
        Serial.printf("%02d/%02d/%04d %02d:%02d:%02d\n", agora.day(), agora.month(), agora.year(), agora.hour(), agora.minute(), agora.second());
    } else {
        Serial.println("[NTP] Falha ao obter horario via NTP.");
    }
}