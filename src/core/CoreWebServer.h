#ifndef CORE_WEB_SERVER_H
#define CORE_WEB_SERVER_H

#include <ESP8266WebServer.h>
#include <LittleFS.h>

/**
 * CoreWebServer - ESP8266 HTTP サーバーのコア基盤
 *
 * 担当する機能:
 *  - 静的ファイル配信 (LittleFS)
 *  - CORS対応 / OPTIONSハンドリング
 *  - キャプティブポータルリダイレクト (APモード時)
 *  - 404レスポンス
 *  - AppAPI が routes を追加するための server() アクセサ提供
 */
class CoreWebServer {
public:
    CoreWebServer();

    // captivePortalMode=true の場合、未知URLをsetup.htmlへリダイレクト
    void begin(bool captivePortalMode = false);
    void handle();

    // AppAPIがルートを登録するために使用
    ESP8266WebServer& server() { return _server; }

    // CORSヘッダ付きJSONレスポンス送信ヘルパー
    void sendJson(int code, const String& json);

private:
    ESP8266WebServer _server;
    bool _isCaptivePortal;
};

#endif
