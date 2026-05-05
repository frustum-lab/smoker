#include "meross_control.h"

MerossControlModule::MerossControlModule(NTPModule& ntp, MD5Module& md5) : _ntp(ntp), _md5(md5) {}

void MerossControlModule::setDeviceInfo(const String& uuid, const String& deviceKey, const String& ip) {
    _uuid = uuid;
    _deviceKey = deviceKey;
    _targetIp = ip;
}

String MerossControlModule::generateMsgId() {
    String res = "";
    const char* hex = "0123456789abcdef";
    for (int i = 0; i < 32; i++) res += hex[random(16)];
    return res;
}

bool MerossControlModule::toggle(bool onoff) {
    unsigned long ts = _ntp.getTime();
    if (ts == 0) {
        Serial.println("[MEROSS] NTP time sync failed");
        return false;
    }

    WiFiClient client;
    client.setTimeout(3000);
    if (!client.connect(_targetIp.c_str(), 80)) {
        Serial.printf("[MEROSS] Connection to %s failed\n", _targetIp.c_str());
        return false;
    }

    String msgId = generateMsgId();
    String sign = _md5.calculateSign(msgId, _deviceKey, ts);
    String body = "{\"header\":{\"from\":\"http://" + _targetIp + "/config\",\"messageId\":\"" + msgId +
                  "\",\"method\":\"SET\",\"namespace\":\"Appliance.Control.ToggleX\",\"payloadVersion\":1,\"sign\":\"" + sign +
                  "\",\"timestamp\":" + String(ts) + ",\"triggerSrc\":\"AndroidLocal\",\"uuid\":\"" + _uuid +
                  "\"},\"payload\":{\"togglex\":{\"channel\":0,\"onoff\":" + String(onoff ? 1 : 0) + "}}}";

    client.print(String("POST /config HTTP/1.1\r\nHost: ") + _targetIp + "\r\nContent-Type: application/json; charset=UTF-8\r\n" +
                 "Content-Length: " + body.length() + "\r\nConnection: close\r\nUser-Agent: okhttp/5.0.0-alpha.14\r\n\r\n" + body);

    unsigned long t = millis();
    bool success = false;
    while (client.connected() && millis() - t < 3000) {
        if (client.available()) {
            String line = client.readStringUntil('\n');
            if (line.indexOf("\"method\":") > -1) {
                success = line.indexOf("SETACK") > -1;
                Serial.printf("[MEROSS] Toggle %s: %s\n", onoff ? "ON" : "OFF", success ? "SUCCESS" : "FAILED");
                break;
            }
        }
    }
    client.stop();
    return success;
}