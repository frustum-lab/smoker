#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

#include <Arduino.h>

// EEPROM address map - Core area (0-95)
#define SC_EEPROM_SIZE   512
#define SC_SSID_ADDR     0
#define SC_SSID_LEN      32
#define SC_PWD_ADDR      32
#define SC_PWD_LEN       64

class SystemConfig {
public:
    static void begin();
    static void saveWiFi(const String& ssid, const String& password);
    static void readWiFi(String& ssid, String& password);
private:
    static void writeStr(int addr, int maxLen, const String& val);
    static String readStr(int addr, int maxLen);
};

#endif
