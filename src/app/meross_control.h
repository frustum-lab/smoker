#ifndef MEROSS_CONTROL_MODULE_H
#define MEROSS_CONTROL_MODULE_H

#include <WiFiClient.h>
#include "ntp.h"
#include "md5.h"

class MerossControlModule {
public:
    MerossControlModule(NTPModule& ntp, MD5Module& md5);
    void setDeviceInfo(const String& uuid, const String& deviceKey, const String& ip);
    bool toggle(bool onoff);
private:
    NTPModule& _ntp;
    MD5Module& _md5;
    String _uuid, _deviceKey, _targetIp;
    String generateMsgId();
};

#endif