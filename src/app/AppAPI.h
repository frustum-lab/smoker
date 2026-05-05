#ifndef APP_API_H
#define APP_API_H

#include "../core/CoreWebServer.h"
#include "temp_control.h"
#include "meross_control.h"
#include "../core/log_module.h"

/**
 * AppAPI - Smoker固有のHTTP APIルート群
 *
 * CoreWebServer に対してルートを登録する。
 * 新しいセンサや制御対象に派生させる場合は、
 * このクラスを置き換えるか拡張する。
 */
class AppAPI {
public:
    AppAPI(CoreWebServer& core, TempControlModule& tempControl,
           MerossControlModule& meross, LogModule& logModule);

    // CoreWebServerのbegin()を呼ぶ前に実行する
    void registerRoutes();

private:
    CoreWebServer&      _core;
    TempControlModule&  _tempControl;
    MerossControlModule& _meross;
    LogModule&          _logModule;

    void handleSetup();
    void handleTempControl();
    void handleForceOn();
    void handleMonitorStart();
    void handleIdle();
    void handleStatus();
    void handleSetDevice();
    void handleSetWiFi();
    void handleGetConfig();
    void handleSetPIDConfig();
    void handleGetLog();
    void handleClearLog();
};

#endif
