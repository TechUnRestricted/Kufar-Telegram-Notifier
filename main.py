import random
from time import sleep

from Configurations.KufarAPI import Kufar
from service import *

# Вывод информации из Конфигурации Kufar
print(f"""
[Kufar:] <- Current Settings ->
   :|Queries           |: {Kufar.query_queries}
   :|Language          |: {Kufar.lang_language}
   :|Price Range       |: {Kufar.prc_priceRange}
   :|Areas             |: {Kufar.ar_areas}
   :|Title-only Search:|: {Kufar.ot_searchOnlyByTitle}
""")

# Вывод информации из Конфигурации Telegram
print(f"""
[Telegram:] <- Current Settings ->
   :|Token  |: {TelegramAPI.token}
   :|Chat ID|: {TelegramAPI.chat_id}
""")

scriptStartTimestamp = getCurrentTimestamp() - 3600  #Отнимаем от текущей даты 1 час
                                                     #для подгрузки более ранних объявлений

while True:
    sleeptime_general = random.randint(30 * 60, 60 * 60)
    sleeptime_query_array = random.randint(15, 60)

    for query in Kufar.query_queries:
        try:
            completeUrl = Kufar.baseurl + quote(query)
            json = getJsonFromURL(completeUrl)
            for item in json["ads"]:
                # print(f"Дата в объявлении: {timestampToReadableDate(zuluDateToTimestamp(item['list_time']))}\n"
                #      f"Дата на сервере  : {timestampToReadableDate(getCurrentTimestamp())}")
                if zuluDateToTimestamp(item["list_time"]) >= scriptStartTimestamp:
                    sellerName = ""
                    images = []

                    # Пытаемся получить имя продавца
                    for accountParameter in item["account_parameters"]:
                        if accountParameter["p"] == "name":
                            sellerName = accountParameter["v"]

                    # Пытаемся получить фото из объявления
                    for image in item["images"]:
                        if image["yams_storage"]:
                            images.append(
                                f"https://yams.kufar.by/api/v1/kufar-ads/images/{image['id'][:2]}/{image['id']}.jpg?rule=pictures")

                    # Проверяем наличие ID объявления в базе данных
                    # Если ID имеется, то объявление не помечается, как новое и не отправляется в Telegram
                    if not checkIfExists(item["ad_id"]):
                        print(f"[New]:> Adding [Title: |{item['subject']}|] [ID: |{item['ad_id']}|] [Tag: |{query}|].")

                        # Добавляем ID объявления в базу данных, чтобы оно не было "новым" на следующий цикл
                        uploadToDatabase(kufar_time=zuluDateToTimestamp(item["list_time"]),
                                         server_time=getCurrentTimestamp(), kufar_id=item["ad_id"], query=query)

                        # Отправка объявления в Telegram
                        sendMessageTelegram(query_tag=query,
                                            title=item["subject"],
                                            price=item["price_byn"],
                                            seller_name=sellerName,
                                            date=item["list_time"],
                                            is_phone_number_hidden=item["phone_hidden"],
                                            link=item["ad_link"],
                                            images=images)
                        sleep(1)
                    else:
                        # Объявление было ранее отправлено в Telegram. Ничего не делаем.
                        pass
                else:
                    pass
                    # Объявление появилось значительно раньше, чем запустился сервер. Ничего не делаем.

            # Небольшая задержка перед запросом следующего ключевого слова
            sleep(sleeptime_query_array)
        except Exception as e:
            print(f"[ERROR]:> An Error has occurred while trying to get data from json[\"ads\"]. |||LOG|||:{e}")

    # Задержка перед повторным запуском поиска.
    print(f"[DEBUG]:> Sleeping for {sleeptime_general / 60} mins.")
    sleep(sleeptime_general)
