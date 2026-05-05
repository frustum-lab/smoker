#ifndef MAX6675_MODULE_H
#define MAX6675_MODULE_H

#include <Arduino.h>

class MAX6675Module {
public:
    MAX6675Module(int cs, int clk, int so);
    void begin();
    float readTempC();
private:
    int _cs, _clk, _so;
    uint16_t readRaw();
};

#endif