# Smoker

ESP8266 (ESP-WROOM-02) を使用して、ネットワーク経由で対象デバイス（スマートプラグなど）のON/OFFを制御するプロジェクトです。

## 概要

このプロジェクトはPlatformIOで構築されています。指定されたWi-Fiネットワークに接続し、ターゲットとなるIPアドレス（例: `192.168.0.2`）のポート80に対してHTTP POSTリクエスト（JSON形式のペイロード）を送信することで、デバイスのトグル操作を行います。

## 環境構築

- **Platform**: Espressif 8266
- **Board**: ESP-WROOM-02
- **Framework**: Arduino
- **Development Environment**: PlatformIO

## セットアップ手順

1. リポジトリをクローンします。
2. PlatformIOがインストールされたVSCode等のエディタでプロジェクトを開きます。
3. `src/main.cpp` などの設定ファイル（Wi-FiのSSID、パスワード、ターゲットIPなど）を環境に合わせて変更してください。
4. ESP8266をPCに接続し、ビルドおよび書き込み（Upload）を実行します。

## 依存ライブラリ
- `ESP8266WiFi`
- `ESP8266HTTPClient`

## ライセンス

This project is open source and available under the [MIT License](LICENSE).
