#include "md5.h"

String MD5Module::calculateSign(const String& msgId, const String& deviceKey, unsigned long ts) {
    MD5Builder md5;
    md5.begin();
    md5.add(msgId + deviceKey + String(ts));
    md5.calculate();
    return md5.toString();
}