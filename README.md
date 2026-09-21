# ⏰ Smart Alarm Clock — ESP32

A smart desk alarm clock built on the ESP32 microcontroller.

Displays real-time clock and date, fires alarms with 8-bit melodies, and allows full configuration from your phone via a web interface accessible through a QR code.

---

## Features

- Real-time clock and date via DS3231 RTC module
- 3 simultaneous configurable alarms with name, icon and melody
- Responsive web interface accessible from any phone via QR code generated at boot
- 8-bit RTTTL melodies (Super Mario, Star Wars, Harry Potter) played on passive buzzer
- Alarm persistence in ESP32 flash memory via `Preferences.h` (no SD card needed)
- Dedicated screens: boot splash, welcome, QR code, main clock and alarm firing
- Capacitive touch control (TTP223): short tap = snooze 5 min, long press = dismiss alarm
- NTP time sync at boot (Wi-Fi); RTC keeps time without internet

---

## Hardware

| Component | Model | Qty |
|---|---|---|
| Microcontroller | ESP32 DevKit 38-pin USB-C | 1 |
| TFT Display | ILI9341 2.8" SPI | 1 |
| RTC Module | DS3231 (with CR2032 battery) | 1 |
| Buzzer | Passive buzzer 5V | 1 |
| Touch Sensor | TTP223 capacitive | 1 |
| Wires | Jumper wires MxM, MxF, FxF | ~20 |
| Power | USB cable or 5V supply | 1 |

---

## Pin Mapping

| Component | ESP32 Pin |
|---|---|
| TFT DC | GPIO 5 |
| TFT CS | GPIO 15 |
| TFT RST | GPIO 4 |
| TFT MOSI | GPIO 23 |
| TFT SCK | GPIO 18 |
| TFT VCC | 3.3V |
| TFT LED | 5V (or 3.3V) |
| RTC SDA | GPIO 21 |
| RTC SCL | GPIO 22 |
| Buzzer | GPIO 17 |
| TTP223 | GPIO 13 |

---

## Software Dependencies

### Tools
- [VS Code](https://code.visualstudio.com/)
- [PlatformIO IDE](https://platformio.org/install/ide?install=vscode) (VS Code extension)

### Libraries (auto-installed via `platformio.ini`)
- `bodmer/TFT_eSPI` — display driver
- `adafruit/RTClib` — DS3231 RTC module
- `adafruit/Adafruit BusIO` — RTClib dependency
- `arduino-libraries/NTPClient` — internet time sync
- `ricmoo/QRCode` — QR code generation

---

## Getting Started

### 1. Clone the repository

```bash
git clone https://github.com/seu-usuario/relogio-smart-esp32.git
cd relogio-smart-esp32
```

### 2. Configure Wi-Fi credentials

Copy the example config file and fill in your credentials:

```bash
cp src/config.example.h src/config.h
```

Edit `src/config.h`:

```cpp
#define WIFI_SSID "your_network_name"
#define PASSWORD  "your_password"
```

### 3. Display setup (TFT_eSPI)

After the first build, PlatformIO will download TFT_eSPI automatically. Pin configuration is already set in `platformio.ini` via `build_flags` — no need to edit the library's `User_Setup.h`.

### 4. Build and upload

Open the project in VS Code with PlatformIO installed, connect the ESP32 via USB and click **Upload** (→) in the PlatformIO bottom bar.

Or via terminal:

```bash
pio run --target upload
```

### 5. Using the device

1. On boot, the display shows a splash screen while connecting to Wi-Fi
2. After connecting, a QR code appears with the device's IP address
3. Scan the QR code with your phone to access the web interface
4. Configure alarms, choose icons and melodies, and click **Save**
5. Alarms are saved to flash and persist after power off

---

## Project Structure

```
relogio-smart-esp32/
├── platformio.ini          # Project config and library dependencies
└── src/
    ├── main.cpp            # Setup and main loop
    ├── config.h            # Pin definitions and credentials (not versioned)
    ├── config.example.h    # Configuration template
    ├── alarm.h / .cpp      # Alarm logic and flash persistence
    ├── display.h / .cpp    # TFT display screens
    ├── rtc.h / .cpp        # DS3231 RTC read and sync
    ├── wifi_ntp.h / .cpp   # Wi-Fi connection and NTP sync
    ├── buzzer.h / .cpp     # RTTTL parser and melody playback
    ├── touch.h / .cpp      # Short/long press detection on TTP223
    └── webserver.h / .cpp  # HTTP server and embedded web interface
```

---

## Web Interface

The web interface is served directly by the ESP32 on port 80. After scanning the QR code, the user sees 3 alarm cards — each with:

- Time (HH:MM)
- Alarm name
- Icon (💼 Work, 💊 Medicine, 🏋️ Exercise, 📚 Study, ☕ Coffee, 🔔 General)
- Melody (Super Mario, Star Wars, Harry Potter)
- Active/inactive toggle

---

## Display Screens

| Screen | Description |
|---|---|
| Splash | Boot logo + "Connecting to Wi-Fi..." |
| Welcome | Message shown after connecting |
| QR Code | Device IP for web access |
| Clock | Real-time clock, date and next alarm |
| Alarm | Dedicated screen when an alarm fires |

---

## Built by

Victor Furlan — Técnico em Desenvolvimento de Sistemas, ETEC Hortolândia