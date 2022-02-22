from service import *

last_offset = 0


class TextTemplates:

    def _start(self, first_name):
        return f"""
Здравствуйте, {first_name}!

Kufar Telegram Notifier (Lite) является ботом,
который раз в заданное время ведет опрос официального Kufar API для поиска новых объявлений
по заданным ключевым словам для последующей отправки результатов в Telegram чат.
Использование данного бота предусматривается только для ознакомительных целей.

Получить помощь: /help
Редактировать список ключевых слов: /list
Настроить интервал проверки объявлений: /changedelay

(или воспользуйтесь виртуальной клавиатурой Telegram)"""

    def _help(self, first_name):
        return """
Получить стартовое сообщение: /start
Получить помощь: /help
Редактировать список ключевых слов: /list
Настроить интервал проверки объявлений: /changedelay

(или воспользуйтесь виртуальной клавиатурой Telegram)
"""


def get_updates(offset=0, limit=100):
    json = get_json_from_url(
        f"{telegram_base_url}/getUpdates?"
        f"offset={offset}&"
        f"limit={limit}"
    )
    return json


def loop_updates_handler(offset=0):
    updates = get_updates(offset=offset)
    for update in updates.get("result", []):
        print(update)
        offset = update.get("update_id", offset) + 1
        events_processing(update)
    sleep(1)
    loop_updates_handler(offset=offset)


def events_processing(event):
    try:
        _message = event.get("message")
        _text = _message.get("text")

        if _text is None:
            return

        _text = _text.lower()
        _from = _message.get("from")
        _chat = _message.get("chat")
        _id = _chat.get("id")
        _first_name = _from.get("first_name", "Unknown")

        if _text == "/start":
            send_message(chat_id=_id, text=TextTemplates()._start(first_name=_first_name))
            pass
        elif _text == "/list":
            pass
        elif _text == "/help":
            send_message(chat_id=_id, text=TextTemplates()._help(first_name=_first_name))
            pass
    except AttributeError as error:
        logging("ERROR", "events_processing", f"Can't properly handle incoming event. [{error}]")
        pass


def send_message(text, chat_id, keyboard_layout=None):
    if text is None or chat_id is None:
        logging("ERROR", "send_message()", "'text' or 'chat_id' is None.")
        return

    return get_json_from_url(f"{telegram_base_url}/sendMessage?"
                             f"chat_id={chat_id}&"
                             f"text={quote(text)}")
