#ifndef MD5_MODULE_H
#define MD5_MODULE_H

#include <MD5Builder.h>

class MD5Module {
public:
    String calculateSign(const String& msgId, const String& deviceKey, unsigned long ts);
};

#endif