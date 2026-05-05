#include "serial_cmd.h"
#include "../core/SystemConfig.h"
#include "AppConfig.h"

SerialCmdModule::SerialCmdModule(TempControlModule& tempControl, MerossControlModule& meross) 
    : _tempControl(tempControl), _meross(meross) {}

void SerialCmdModule::processInput() {
    while (Serial.available()) {
        char c = Serial.read();
        if (c == '\n' || c == '\r') {
            if (_buffer.length() > 0) {
                processCommand(_buffer);
                _buffer = "";
            }
        } else {
            _buffer += c;
        }
    }
}

void SerialCmdModule::processCommand(const String& cmd) {
    if (cmd.startsWith("t")) {
        String argsStr = cmd.substring(1);
        argsStr.trim();
        float target = argsStr.toFloat();

        if (target > 0 && target <= 500) {
            _tempControl.setTargetTemp(target);
            _tempControl.startTempControl();
            Serial.printf("Temperature control started: %.1f°C\n", target);
        } else {
            Serial.println("Invalid temperature. Use: t<temp> (e.g. t200)");
        }
    } else if (cmd == "o") {
        _tempControl.startForceOn();
        Serial.println("Force ON started");
    } else if (cmd == "m") {
        _tempControl.startMonitoring();
        Serial.println("Monitoring started");
    } else if (cmd == "i") {
        _tempControl.stop();
        Serial.println("Idle mode");
    } else if (cmd.startsWith("cw ")) {
        int space1 = cmd.indexOf(' ');
        int space2 = cmd.indexOf(' ', space1 + 1);
        if (space1 > -1 && space2 > -1) {
            String ssid = cmd.substring(space1 + 1, space2);
            String pwd = cmd.substring(space2 + 1);
            SystemConfig::saveWiFi(ssid, pwd);
            Serial.printf("WiFi Config Saved! SSID: %s. Rebooting...\n", ssid.c_str());
            delay(1000);
            ESP.restart();
        } else {
            Serial.println("Usage: cw <SSID> <PASSWORD>");
        }
    } else if (cmd.startsWith("cm ")) {
        int s1 = cmd.indexOf(' ');
        int s2 = cmd.indexOf(' ', s1 + 1);
        int s3 = cmd.indexOf(' ', s2 + 1);
        if (s1 > -1 && s2 > -1 && s3 > -1) {
            String uuid = cmd.substring(s1 + 1, s2);
            String key = cmd.substring(s2 + 1, s3);
            String ip = cmd.substring(s3 + 1);
            _meross.setDeviceInfo(uuid, key, ip);
            AppConfig::saveMerossDevice(uuid, key, ip);
            Serial.printf("Meross Config Saved! IP: %s\n", ip.c_str());
        } else {
            Serial.println("Usage: cm <UUID> <KEY> <IP>");
        }
    } else if (cmd == "c") {
        String ssid, pass, uuid, key, ip;
        SystemConfig::readWiFi(ssid, pass);
        AppConfig::readMerossDevice(uuid, key, ip);
        Serial.println("--- Current Config ---");
        Serial.printf(" WiFi SSID: %s\n", ssid.c_str());
        Serial.printf(" WiFi Pass: %s\n", pass.length() > 0 ? "********" : "(empty)");
        Serial.printf(" Meross UUID: %s\n", uuid.c_str());
        Serial.printf(" Meross Key : %s\n", key.length() > 0 ? "********" : "(empty)");
        Serial.printf(" Meross IP  : %s\n", ip.c_str());
        Serial.println("----------------------");
    } else if (cmd == "s") {
        ControlState state = _tempControl.getState();
        float temp = _tempControl.getCurrentTemp();
        String stateStr = (state == IDLE) ? "idle" : (state == TEMP_CONTROL) ? "temp_control" : (state == FORCE_ON) ? "force_on" : "monitoring";
        Serial.printf("State: %s, Current Temp: %.2f°C, Target: %.1f°C\n", stateStr.c_str(), temp, _tempControl.getTargetTemp());
    } else if (cmd == "h" || cmd == "help") {
        Serial.println("Available commands:");
        Serial.println("  t<temp>        - Temp control (e.g., t60)");
        Serial.println("  o              - Force heater ON");
        Serial.println("  m              - Start monitoring mode");
        Serial.println("  i              - Idle mode (heater OFF)");
        Serial.println("  s              - Show current status");
        Serial.println("  c              - Show current config");
        Serial.println("  cw <ssid> <pw> - Set WiFi info (Requires restart)");
        Serial.println("  cm <id> <key> <ip> - Set Meross info");
        Serial.println("  h/help         - Show this help");
    } else {
        Serial.printf("Unknown command: %s\n", cmd.c_str());
        Serial.println("Type 'h' or 'help' for available commands");
    }
}