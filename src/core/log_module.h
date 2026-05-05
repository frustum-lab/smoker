#ifndef LOG_MODULE_H
#define LOG_MODULE_H

#include <LittleFS.h>
#include <Arduino.h>

class LogModule {
public:
    LogModule();
    void begin();
    void logData(float currentTemp, float targetTemp, int heaterState);
    String getLogFilePath();
    void clearLog();

private:
    const char* _logFilePath = "/smoker_log.csv";
    unsigned long _lastLogTime;
    const unsigned long _logInterval = 60000; // Log every 60 seconds (1 minute)
    const size_t _maxLogSize = 54000; // Max size: ~54KB (approx 30 hours at 1-min intervals)
    void writeHeaderIfNeeded();
};

#endif // LOG_MODULE_H
