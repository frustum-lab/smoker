#ifndef SERIAL_CMD_MODULE_H
#define SERIAL_CMD_MODULE_H

#include "temp_control.h"
#include "meross_control.h"

class SerialCmdModule {
public:
    SerialCmdModule(TempControlModule& tempControl, MerossControlModule& meross);
    void processInput();
private:
    TempControlModule& _tempControl;
    MerossControlModule& _meross;
    String _buffer;
    void processCommand(const String& cmd);
};

#endif