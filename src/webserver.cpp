#include <Arduino.h>
#include <WiFi.h>
#include "C:/Users/victo/.platformio/packages/framework-arduinoespressif32/libraries/WebServer/src/WebServer.h"
#include "webserver.h"
#include "config.h"
#include "alarm.h"

extern Alarme alarmes[3];

WebServer server(80);

void inicializeWebServer() {
    server.on("/", [&]() {
        server.send(200, "text/html", R"rawstring(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Relógio Smart</title>
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
        .toast {
            display: none;
            position: fixed;
            bottom: 24px;
            left: 50%;
            transform: translateX(-50%);
            background: #7c3aed;
            color: #fff;
            padding: 12px 24px;
            border-radius: 999px;
            font-size: 0.9rem;
        }
    </style>
</head>
<body>
    <h1>⏰ Relógio Smart</h1>
    <form action="/salvar" method="POST">
        <div class="card">
            <div class="card-header">
                <span class="card-title">Alarme 1</span>
                <label class="toggle">
                    <input type="checkbox" name="a0ativo" value="1">
                    <span class="toggle-slider"></span>
                </label>
            </div>
            <div class="field"><label>Horário</label><input type="time" name="a0hora"></div>
            <div class="field"><label>Nome</label><input type="text" name="a0nome" placeholder="Ex: Acordar"></div>
            <div class="field"><label>Ícone</label>
                <select name="a0icone">
                    <option value="💼">💼 Trabalho</option>
                    <option value="💊">💊 Remédio</option>
                    <option value="🏋️">🏋️ Exercício</option>
                    <option value="📚">📚 Estudos</option>
                    <option value="☕">☕ Café</option>
                    <option value="🔔">🔔 Geral</option>
                </select>
            </div>
            <div class="field"><label>Melodia</label>
                <select name="a0melodia">
                    <option value="mario">Super Mario</option>
                    <option value="starwars">Star Wars</option>
                    <option value="harrypotter">Harry Potter</option>
                </select>
            </div>
        </div>
        <div class="card">
            <div class="card-header">
                <span class="card-title">Alarme 2</span>
                <label class="toggle">
                    <input type="checkbox" name="a1ativo" value="1">
                    <span class="toggle-slider"></span>
                </label>
            </div>
            <div class="field"><label>Horário</label><input type="time" name="a1hora"></div>
            <div class="field"><label>Nome</label><input type="text" name="a1nome" placeholder="Ex: Remédio"></div>
            <div class="field"><label>Ícone</label>
                <select name="a1icone">
                    <option value="💼">💼 Trabalho</option>
                    <option value="💊">💊 Remédio</option>
                    <option value="🏋️">🏋️ Exercício</option>
                    <option value="📚">📚 Estudos</option>
                    <option value="☕">☕ Café</option>
                    <option value="🔔">🔔 Geral</option>
                </select>
            </div>
            <div class="field"><label>Melodia</label>
                <select name="a1melodia">
                    <option value="mario">Super Mario</option>
                    <option value="starwars">Star Wars</option>
                    <option value="harrypotter">Harry Potter</option>
                </select>
            </div>
        </div>
        <div class="card">
            <div class="card-header">
                <span class="card-title">Alarme 3</span>
                <label class="toggle">
                    <input type="checkbox" name="a2ativo" value="1">
                    <span class="toggle-slider"></span>
                </label>
            </div>
            <div class="field"><label>Horário</label><input type="time" name="a2hora"></div>
            <div class="field"><label>Nome</label><input type="text" name="a2nome" placeholder="Ex: Academia"></div>
            <div class="field"><label>Ícone</label>
                <select name="a2icone">
                    <option value="💼">💼 Trabalho</option>
                    <option value="💊">💊 Remédio</option>
                    <option value="🏋️">🏋️ Exercício</option>
                    <option value="📚">📚 Estudos</option>
                    <option value="☕">☕ Café</option>
                    <option value="🔔">🔔 Geral</option>
                </select>
            </div>
            <div class="field"><label>Melodia</label>
                <select name="a2melodia">
                    <option value="mario">Super Mario</option>
                    <option value="starwars">Star Wars</option>
                    <option value="harrypotter">Harry Potter</option>
                </select>
            </div>
        </div>
        <button type="submit" class="btn-salvar">💾 Salvar Alarmes</button>
    </form>
    <div class="toast" id="toast">✅ Alarmes salvos!</div>
</body>
</html>
        )rawstring");
    });

    server.on("/salvar", HTTP_POST, [&]() {
        for (int i = 0; i < 3; i++) {
            // Horário vem como "HH:MM" — separa hora e minuto
            String horario = server.arg("a" + String(i) + "hora");
            alarmes[i].hora   = horario.substring(0, 2).toInt();
            alarmes[i].minuto = horario.substring(3, 5).toInt();

            // Campos de texto direto
            alarmes[i].nome    = server.arg("a" + String(i) + "nome");
            alarmes[i].icone   = server.arg("a" + String(i) + "icone");
            alarmes[i].melodia = server.arg("a" + String(i) + "melodia");

            // Checkbox só é enviado quando marcado — hasArg verifica se existe
            alarmes[i].ativo = server.hasArg("a" + String(i) + "ativo");
        }

        // Grava tudo na flash
        saveAlarms();

        // Redireciona de volta pra página principal
        server.sendHeader("Location", "/");
        server.send(303);
    });

    server.begin();
}

void handleWebServer() {
    server.handleClient();
}