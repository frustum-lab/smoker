#include "AppConfig.h"
#include <EEPROM.h>

void AppConfig::writeStr(int addr, int maxLen, const String& val) {
    for (int i = 0; i < maxLen; i++) {
        EEPROM.write(addr + i, (i < (int)val.length()) ? val[i] : 0);
    }
    EEPROM.commit();
}

String AppConfig::readStr(int addr, int maxLen) {
    String result = "";
    for (int i = 0; i < maxLen; i++) {
        char c = EEPROM.read(addr + i);
        if (c == 0 || (byte)c == 0xFF) break;
        result += c;
    }
    return result;
}

void AppConfig::saveMerossDevice(const String& uuid, const String& key, const String& ip) {
    writeStr(AC_UUID_ADDR, AC_UUID_LEN, uuid);
    writeStr(AC_KEY_ADDR,  AC_KEY_LEN,  key);
    writeStr(AC_IP_ADDR,   AC_IP_LEN,   ip);
}

void AppConfig::readMerossDevice(String& uuid, String& key, String& ip) {
    uuid = readStr(AC_UUID_ADDR, AC_UUID_LEN);
    key  = readStr(AC_KEY_ADDR,  AC_KEY_LEN);
    ip   = readStr(AC_IP_ADDR,   AC_IP_LEN);
}

void AppConfig::savePIDConfig(float kp, float ki, float kd, unsigned long windowSize) {
    EEPROM.write(AC_PID_FLAG_ADDR, 1);
    EEPROM.put(AC_PID_KP_ADDR,  kp);
    EEPROM.put(AC_PID_KI_ADDR,  ki);
    EEPROM.put(AC_PID_KD_ADDR,  kd);
    EEPROM.put(AC_PID_WIN_ADDR, windowSize);
    EEPROM.commit();
}

bool AppConfig::readPIDConfig(float& kp, float& ki, float& kd, unsigned long& windowSize) {
    if (EEPROM.read(AC_PID_FLAG_ADDR) != 1) return false;
    EEPROM.get(AC_PID_KP_ADDR,  kp);
    EEPROM.get(AC_PID_KI_ADDR,  ki);
    EEPROM.get(AC_PID_KD_ADDR,  kd);
    EEPROM.get(AC_PID_WIN_ADDR, windowSize);
    return true;
}
