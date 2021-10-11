import requests
import uuid
from datetime import datetime
from Configurations.MariaDB import MariaDB
from Configurations.TelegramAPI import TelegramAPI
from urllib.parse import quote

newline = "\n"

requests.packages.urllib3.disable_warnings()


def findBetween(s, start, end):
    return (s.split(start))[1].split(end)[0]


headers = {
    'user-agent': 'Kufar/11.42.1/13.7/iPhone 11',
    'x-device-id': str(uuid.uuid4()),
    'x-app-name': 'iOS Kufar',
    'x-app-version': '11.42.1'
}


def priceFormatted(min, max):
    return f"r:{min * 100},{max * 100}"


def areasFormatted(areas):
    return f"v.or:{','.join(map(str, areas))}"


def zuluDateToTimestamp(date):
    return int(datetime.strptime(date, "%Y-%m-%dT%H:%M:%SZ").timestamp()) + (3 * 3600)


def getCurrentTimestamp():
    return datetime.now().timestamp()


def timestampToReadableDate(timestamp):
    return datetime.fromtimestamp(timestamp)


def getJsonFromURL(url):
    return requests.get(url, headers=headers, verify=False).json()


def checkIfExists(kufar_id):
    cursor = MariaDB.conn.cursor()
    cursor.execute("SELECT * FROM ads WHERE kufar_id=? LIMIT 1", (kufar_id,))

    # True - запись уже есть
    # False - записи нет

    return cursor.fetchone() is not None


def uploadToDatabase(kufar_time, server_time, kufar_id, query):
    cursor = MariaDB.conn.cursor()
    cursor.execute("INSERT INTO ads (kufar_time, server_time, kufar_id, query) VALUES (?, ?, ?, ?)",
                   (kufar_time, server_time, kufar_id, query))
    return


def sendMessageTelegram(query_tag, title, price, seller_name, date, is_phone_number_hidden, link, images):
    message = f"""
#{query_tag}
Title: {title}
Date: {timestampToReadableDate(zuluDateToTimestamp(date))}
Price: {int(price) / 100} BYN

Seller Name: {seller_name}
Phone number is hidden: {is_phone_number_hidden}
Link: {link}
    """

    baseurl = f"https://api.telegram.org/bot{TelegramAPI.token}/sendPhoto?chat_id={TelegramAPI.chat_id}&caption={quote(message)}&photo={quote(images[0] if images else 'https://via.placeholder.com/1080')}"
    print(getJsonFromURL(baseurl))
    return
