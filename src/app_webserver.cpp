#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "app_webserver.h"
#include "config.h"
#include "alarm.h"

extern Alarme alarmes[3];

WebServer server(80);

static String generateHTML() {
    String html = R"rawstring(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Relógio Smart IoT</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body {
            font-family: 'Segoe UI', sans-serif;
            background: #0f0f1a;
            color: #fff;
            min-height: 100vh;
            padding: 20px;
        }
        h1 {
            text-align: center;
            font-size: 1.5rem;
            margin-bottom: 24px;
            color: #a78bfa;
            letter-spacing: 2px;
        }
        .card {
            background: #1a1a2e;
            border: 1px solid #2d2d4e;
            border-radius: 16px;
            padding: 20px;
            margin-bottom: 16px;
        }
        .card-header {
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin-bottom: 16px;
        }
        .card-title {
            font-size: 1rem;
            font-weight: 600;
            color: #a78bfa;
        }
        .toggle {
            position: relative;
            width: 48px;
            height: 26px;
        }
        .toggle input { display: none; }
        .toggle-slider {
            position: absolute;
            inset: 0;
            background: #2d2d4e;
            border-radius: 999px;
            cursor: pointer;
            transition: 0.3s;
        }
        .toggle-slider::before {
            content: '';
            position: absolute;
            width: 20px;
            height: 20px;
            background: #fff;
            border-radius: 50%;
            top: 3px;
            left: 3px;
            transition: 0.3s;
        }
        .toggle input:checked + .toggle-slider { background: #7c3aed; }
        .toggle input:checked + .toggle-slider::before { transform: translateX(22px); }
        .field { margin-bottom: 12px; }
        label {
            display: block;
            font-size: 0.75rem;
            color: #888;
            margin-bottom: 4px;
            text-transform: uppercase;
            letter-spacing: 1px;
        }
        input[type="time"],
        input[type="text"],
        select {
            width: 100%;
            padding: 10px 12px;
            background: #0f0f1a;
            border: 1px solid #2d2d4e;
            border-radius: 8px;
            color: #fff;
            font-size: 0.95rem;
            outline: none;
            transition: border 0.2s;
        }
        input[type="time"]:focus,
        input[type="text"]:focus,
        select:focus { border-color: #7c3aed; }
        select option { background: #1a1a2e; }
        .btn-salvar {
            width: 100%;
            padding: 14px;
            background: #7c3aed;
            color: #fff;
            border: none;
            border-radius: 12px;
            font-size: 1rem;
            font-weight: 600;
            cursor: pointer;
            margin-top: 8px;
            letter-spacing: 1px;
            transition: background 0.2s;
        }
        .btn-salvar:active { background: #6d28d9; }
    </style>
</head>
<body>
    <h1>⏰ Relógio Smart IoT</h1>
    <form action="/salvar" method="POST">
)rawstring";

    for (int i = 0; i < 3; i++) {
        String hStr = alarmes[i].hora < 10 ? "0" + String(alarmes[i].hora) : String(alarmes[i].hora);
        String mStr = alarmes[i].minuto < 10 ? "0" + String(alarmes[i].minuto) : String(alarmes[i].minuto);
        String timeVal = hStr + ":" + mStr;

        html += "<div class=\"card\">";
        html += "<div class=\"card-header\">";
        html += "<span class=\"card-title\">Alarme " + String(i + 1) + "</span>";
        html += "<label class=\"toggle\">";
        html += "<input type=\"checkbox\" name=\"a" + String(i) + "ativo\" value=\"1\" " + (alarmes[i].ativo ? "checked" : "") + ">";
        html += "<span class=\"toggle-slider\"></span>";
        html += "</label>";
        html += "</div>";
        html += "<div class=\"field\"><label>Horário</label><input type=\"time\" name=\"a" + String(i) + "hora\" value=\"" + timeVal + "\"></div>";
        html += "<div class=\"field\"><label>Nome</label><input type=\"text\" name=\"a" + String(i) + "nome\" value=\"" + alarmes[i].nome + "\"></div>";
        
        // Icones
        html += "<div class=\"field\"><label>Ícone</label><select name=\"a" + String(i) + "icone\">";
        const char* icons[] = {"💼", "💊", "🏋️", "📚", "☕", "🔔"};
        const char* iconLabels[] = {"💼 Trabalho", "💊 Remédio", "🏋️ Exercício", "📚 Estudos", "☕ Café", "🔔 Geral"};
        for (int k = 0; k < 6; k++) {
            String sel = (alarmes[i].icone == String(icons[k])) ? "selected" : "";
            html += "<option value=\"" + String(icons[k]) + "\" " + sel + ">" + String(iconLabels[k]) + "</option>";
        }
        html += "</select></div>";

        // Melodias
        html += "<div class=\"field\"><label>Melodia</label><select name=\"a" + String(i) + "melodia\">";
        const char* mels[] = {"mario", "starwars", "harrypotter"};
        const char* melLabels[] = {"Super Mario", "Star Wars", "Harry Potter"};
        for (int k = 0; k < 3; k++) {
            String sel = (alarmes[i].melodia == String(mels[k])) ? "selected" : "";
            html += "<option value=\"" + String(mels[k]) + "\" " + sel + ">" + String(melLabels[k]) + "</option>";
        }
        html += "</select></div>";
        html += "</div>";
    }

    html += R"rawstring(
        <button type="submit" class="btn-salvar">💾 Salvar Alarmes</button>
    </form>
</body>
</html>
)rawstring";

    return html;
}

void inicializeWebServer() {
    server.on("/", HTTP_GET, [&]() {
        server.send(200, "text/html", generateHTML());
    });

    server.on("/salvar", HTTP_POST, [&]() {
        for (int i = 0; i < 3; i++) {
            String horario = server.arg("a" + String(i) + "hora");
            if (horario.length() >= 5) {
                alarmes[i].hora   = horario.substring(0, 2).toInt();
                alarmes[i].minuto = horario.substring(3, 5).toInt();
            }

            alarmes[i].nome    = server.arg("a" + String(i) + "nome");
            alarmes[i].icone   = server.arg("a" + String(i) + "icone");
            alarmes[i].melodia = server.arg("a" + String(i) + "melodia");
            alarmes[i].ativo   = server.hasArg("a" + String(i) + "ativo");
        }

        saveAlarms();

        server.sendHeader("Location", "/");
        server.send(303);
    });

    server.begin();
    Serial.println("[WEBSERVER] Servidor HTTP iniciado na porta 80.");
}

void handleWebServer() {
    server.handleClient();
}
