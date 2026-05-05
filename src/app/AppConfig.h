#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <Arduino.h>

// EEPROM address map - App area (96-511)
// Must not overlap with SystemConfig (0-95)
#define AC_UUID_ADDR     96
#define AC_UUID_LEN      64
#define AC_KEY_ADDR      160
#define AC_KEY_LEN       64
#define AC_IP_ADDR       224
#define AC_IP_LEN        16
#define AC_PID_FLAG_ADDR 300
#define AC_PID_KP_ADDR   (AC_PID_FLAG_ADDR + 1)
#define AC_PID_KI_ADDR   (AC_PID_KP_ADDR + 4)
#define AC_PID_KD_ADDR   (AC_PID_KI_ADDR + 4)
#define AC_PID_WIN_ADDR  (AC_PID_KD_ADDR + 4)

class AppConfig {
public:
    static void saveMerossDevice(const String& uuid, const String& key, const String& ip);
    static void readMerossDevice(String& uuid, String& key, String& ip);
    static void savePIDConfig(float kp, float ki, float kd, unsigned long windowSize);
    static bool readPIDConfig(float& kp, float& ki, float& kd, unsigned long& windowSize);
private:
    static void writeStr(int addr, int maxLen, const String& val);
    static String readStr(int addr, int maxLen);
};

#endif
