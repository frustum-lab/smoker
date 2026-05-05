#include <Arduino.h>
#include "max6675.h"

MAX6675Module::MAX6675Module(int cs, int clk, int so) : _cs(cs), _clk(clk), _so(so) {}

void MAX6675Module::begin() {
    pinMode(_cs, OUTPUT);
    pinMode(_clk, OUTPUT);
    pinMode(_so, INPUT);
    digitalWrite(_cs, HIGH);
    digitalWrite(_clk, LOW);
}

uint16_t MAX6675Module::readRaw() {
    uint16_t value = 0;
    digitalWrite(_cs, LOW);
    delayMicroseconds(10);
    for (int i = 15; i >= 0; i--) {
        digitalWrite(_clk, LOW);
        delayMicroseconds(1);
        value <<= 1;
        if (digitalRead(_so)) value |= 0x01;
        digitalWrite(_clk, HIGH);
        delayMicroseconds(1);
    }
    digitalWrite(_cs, HIGH);
    return value;
}

float MAX6675Module::readTempC() {
    uint16_t raw = readRaw();
    if (raw & 0x0004) {
        Serial.println("[MAX6675] Thermocouple not connected or faulty");
        return -999; // Error
    }
    if (raw & 0x0002) {
        Serial.println("[MAX6675] Thermocouple shorted to GND");
        return -998; // Short to GND
    }
    if (raw & 0x0001) {
        Serial.println("[MAX6675] Thermocouple shorted to VCC");
        return -997; // Short to VCC
    }
    raw >>= 3;
    float temp = raw * 0.25;
    if (temp < -200 || temp > 1350) {
        Serial.printf("[MAX6675] Temperature out of range: %.2f°C\n", temp);
        return -996; // Out of range
    }
    return temp;
}