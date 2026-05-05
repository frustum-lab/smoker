#include "CoreWebServer.h"

CoreWebServer::CoreWebServer() : _server(80), _isCaptivePortal(false) {}

void CoreWebServer::begin(bool captivePortalMode) {
    _isCaptivePortal = captivePortalMode;

    // 静的ファイル配信 (LittleFS上のWebUI)
    _server.serveStatic("/", LittleFS, "/");

    // 未登録URLのハンドリング
    _server.onNotFound([this]() {
        if (_server.method() == HTTP_OPTIONS) {
            // CORS preflight
            _server.sendHeader("Access-Control-Allow-Origin", "*");
            _server.sendHeader("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
            _server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
            _server.send(204);
        } else if (_isCaptivePortal) {
            // APモード: setup.htmlへリダイレクト
            _server.sendHeader("Location", "http://192.168.4.1/setup.html", true);
            _server.send(302, "text/plain", "Redirecting to setup");
        } else {
            _server.sendHeader("Access-Control-Allow-Origin", "*");
            _server.send(404, "application/json", "{\"error\":\"not_found\"}");
        }
    });

    _server.begin();
    Serial.println("[CoreWebServer] HTTP server started on port 80");
}

void CoreWebServer::handle() {
    _server.handleClient();
}

void CoreWebServer::sendJson(int code, const String& json) {
    _server.sendHeader("Access-Control-Allow-Origin", "*");
    _server.send(code, "application/json", json);
}
