import argparse
import asyncio
import os
import requests
from datetime import datetime
from meross_iot.http_api import MerossHttpClient, MerossCloudCreds
from meross_iot.manager import MerossManager

async def main(email, password):
    print("Merossクラウドにログイン中...")
    api_base_url = "https://iot.meross.com"
    dummy_creds = MerossCloudCreds(user_id="", user_email=email, issued_on=datetime.now().isoformat(), domain=api_base_url, mqtt_domain="", token="", key="")
    http_client = MerossHttpClient(cloud_credentials=dummy_creds)

    try:
        creds = await http_client.async_login(email=email, password=password, api_base_url=api_base_url)
        if not creds:
            creds = http_client.cloud_credentials
    except Exception as e:
        print(f"❌ ログイン失敗: {e}")
        return

    print("デバイスを検索中（数秒かかります）...")
    manager = MerossManager(http_client=MerossHttpClient(cloud_credentials=creds))
    await manager.async_init()
    devices = await manager.async_device_discovery()
    
    if not devices:
        print("❌ デバイスが見つかりませんでした。")
        manager.close()
        return

    dev = devices[0]
    print(f"\n✅ デバイス「{dev.name}」を発見！")
    
    # 情報の抽出
    key_val = creds.key
    uuid_val = dev.uuid
    ip_val = None
    
    if hasattr(dev, 'lan_ip') and dev.lan_ip:
        ip_val = dev.lan_ip
    elif hasattr(dev, 'host'):
        ip_val = dev.host
        
    print(f"  UUID : {uuid_val}")
    print(f"  Key  : ******** (取得成功)")
    print(f"  IP   : {ip_val if ip_val else '自動取得失敗'}")

    manager.close()

    # 自動取得できなかった場合のフォールバック
    if not ip_val:
        ip_val = input("\nプラグのIPアドレスを手動で入力してください (例: 192.168.0.2): ")
        
    # ESP8266への無線送信
    esp_url = "http://smoker.local/config/device"
    payload = {
        "uuid": uuid_val,
        "key": key_val,
        "ip": ip_val
    }
    
    print(f"\nESP8266 ({esp_url}) に設定データを送信中...")
    try:
        response = requests.post(esp_url, data=payload, timeout=10)
        if response.status_code == 200:
            print("✨ 完了！シリアル接続なしでESP8266にキーが記憶されました！")
        else:
            print(f"❌ 失敗: ESP8266がエラーを返しました ({response.status_code})\n詳細: {response.text}")
    except Exception as e:
        print(f"❌ 通信エラー: ESP8266が見つかりません。")
        print("  ・ESP8266の電源が入っているか\n  ・PCと同じWi-Fiネットワークに繋がっているかを確認してください。")

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Meross設定取得＆ESP8266自動転送スクリプト")
    parser.add_argument("email", help="Merossアカウントのメールアドレス")
    parser.add_argument("password", help="Merossアカウントのパスワード")
    args = parser.parse_args()

    if os.name == 'nt':
        asyncio.set_event_loop_policy(asyncio.WindowsSelectorEventLoopPolicy())
    asyncio.run(main(args.email, args.password))