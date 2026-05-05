#include "ntp.h"

NTPModule::NTPModule() {
}

void NTPModule::begin() {
    udp.begin(NTP_PORT);
}

unsigned long NTPModule::getTime() {
    unsigned long currentMillis = millis();
    // 1時間(3600000ms)以内であれば自前で計算した時間を返す
    if (_lastTimeSync != 0 && (currentMillis - _lastMillis < 3600000)) {
        return _lastTimeSync + ((currentMillis - _lastMillis) / 1000);
    }

    byte packet[48] = {};
    packet[0] = 0b11100011;
    packet[1] = 0; packet[2] = 6; packet[3] = 0xEC;
    packet[12] = 49; packet[13] = 0x4E; packet[14] = 49; packet[15] = 52;

    udp.beginPacket(NTP_SERVER, NTP_PORT);
    udp.write(packet, 48);
    udp.endPacket();

    unsigned long startWait = millis();
    while (millis() - startWait < NTP_TIMEOUT_MS) {
        if (udp.parsePacket()) {
            udp.read(packet, 48);
            unsigned long hi = word(packet[40], packet[41]);
            unsigned long lo = word(packet[42], packet[43]);
            unsigned long timestamp = (hi << 16 | lo) - 2208988800UL;
            _lastTimeSync = timestamp;
            _lastMillis = millis();
            return timestamp;
        }
        delay(10);
    }
    
    // 通信エラー時でも古い同期時間があれば計算して継続
    if (_lastTimeSync != 0) {
        return _lastTimeSync + ((currentMillis - _lastMillis) / 1000);
    }
    return 0;
}