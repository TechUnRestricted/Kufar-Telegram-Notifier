from datetime import datetime
from time import sleep
from urllib.parse import quote

import requests
import uuid
import settings

requests.packages.urllib3.disable_warnings()

telegram_base_url = f"https://api.telegram.org/bot{settings.TelegramAPI.token}"

headers = {
    'user-agent': 'Kufar/11.42.1/13.7/iPhone 11',
    'x-device-id': str(uuid.uuid4()),
    'x-app-name': 'iOS Kufar',
    'x-app-version': '11.42.1'
}

def logging(type, defname, message, save_to_disk=False):
    print(f"[{type}: ({defname})]: {message}")

def get_current_timestamp():
    return datetime.now().timestamp()


def get_json_from_url(url):
    return requests.get(url, headers=headers, verify=False).json()