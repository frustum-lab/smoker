#include "SystemConfig.h"
#include <EEPROM.h>

void SystemConfig::begin() {
    EEPROM.begin(SC_EEPROM_SIZE);
}

void SystemConfig::writeStr(int addr, int maxLen, const String& val) {
    for (int i = 0; i < maxLen; i++) {
        EEPROM.write(addr + i, (i < (int)val.length()) ? val[i] : 0);
    }
    EEPROM.commit();
}

String SystemConfig::readStr(int addr, int maxLen) {
    String result = "";
    for (int i = 0; i < maxLen; i++) {
        char c = EEPROM.read(addr + i);
        if (c == 0 || (byte)c == 0xFF) break;
        result += c;
    }
    return result;
}

void SystemConfig::saveWiFi(const String& ssid, const String& password) {
    writeStr(SC_SSID_ADDR, SC_SSID_LEN, ssid);
    writeStr(SC_PWD_ADDR,  SC_PWD_LEN,  password);
}

void SystemConfig::readWiFi(String& ssid, String& password) {
    ssid     = readStr(SC_SSID_ADDR, SC_SSID_LEN);
    password = readStr(SC_PWD_ADDR,  SC_PWD_LEN);
}
