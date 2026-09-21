#include "config.h"
#include "wifi_ntp.h"
#include "rtc.h"
#include <RTClib.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

WiFiUDP udp;

NTPClient ntp(udp, "pool.ntp.org", HORARIO);

void connectWifi() {

    WiFi.begin(WIFI_SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
    }
    
};

void syncNTP() {
    ntp.begin();
    ntp.update();

    DateTime agora = DateTime(ntp.getEpochTime());

    setHour(agora);
}