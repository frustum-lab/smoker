#ifndef NTP_MODULE_H
#define NTP_MODULE_H

#include <WiFiUdp.h>
#include <Arduino.h>

#include "../../include/config.h"

class NTPModule {
public:
    NTPModule();
    void begin();
    unsigned long getTime();
private:
    WiFiUDP udp;
    unsigned long _lastTimeSync = 0;
    unsigned long _lastMillis = 0;
};

#endif