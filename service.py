from datetime import datetime
from time import sleep
from urllib.parse import quote
import urllib.parse

import requests
import uuid
import settings

requests.packages.urllib3.disable_warnings()

telegram_base_url = f"https://api.telegram.org/bot{settings.TelegramAPI.token}"
kufar_base_url = "https://searchapi.kufar.by/v1/search/rendered-paginated?"
time_shift = 3600 * 3
placeholder_url = "https://via.placeholder.com/1080"


def get_current_timestamp():
    return datetime.now().timestamp()


script_start_timestamp = get_current_timestamp()

headers = {
    'user-agent': 'Kufar/11.42.1/13.7/iPhone 11',
    'x-device-id': str(uuid.uuid4()),
    'x-app-name': 'iOS Kufar',
    'x-app-version': '11.42.1'
}


def logger(type, defname, message, save_to_disk=False):
    print(f"[{type}: ({defname})]: {message}")


def zulu_date_to_timestamp(date):
    return int(datetime.strptime(date, "%Y-%m-%dT%H:%M:%SZ").timestamp()) + (3 * 3600)


def get_json_from_url(url):
    debug_json = requests.get(url, headers=headers, verify=False).json()
    print(debug_json)
    return debug_json