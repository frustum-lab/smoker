#ifndef OTA_MODULE_H
#define OTA_MODULE_H

#include <Arduino.h>

/**
 * OTAModule - Wi-Fi経由でのファームウェアアップデート (ArduinoOTA)
 *
 * 使い方:
 *   OTAModule ota;
 *   ota.begin("device-hostname");   // setup()内、WiFi接続後に呼ぶ
 *   ota.handle();                   // loop()内で毎回呼ぶ
 */
class OTAModule {
public:
    // hostname: mDNS/OTAホスト名 (例: "smoker")
    void begin(const char* hostname = nullptr);
    void handle();
};

#endif
