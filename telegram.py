import json

from service import *
import templates


def add_element_to_list(element_name, chat_id, user_id):
    # TODO: add elements and save to file

    element_name = element_name.split('\n', 1)[0].strip()
    if element_name:
        if len(element_name) > settings.TelegramAPI.element_max_length:
            logger("ERROR", "add_element_to_list", f"Can't add new element because the length exceeds {settings.TelegramAPI.element_max_length} char limit.")
            send_message(chat_id=chat_id, text=templates.msg_cant_add_element_exceeds_length())
            return
        send_message(chat_id=chat_id, text=templates.msg_cant_add_element_success(element_name))
        settings.TelegramAPI.list_parse.append(element_name)
    else:
        logger("ERROR", "add_element_to_list", "Can't add new element because it's empty.")
        send_message(chat_id=chat_id, text=templates.msg_cant_add_element_empty())


def get_updates(offset=0, limit=100, timeout=50):
    json = get_json_from_url(
        f"{telegram_base_url}/getUpdates?"
        f"offset={offset}&"
        f"limit={limit}&"
        f"timeout={timeout}"
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
            send_message(chat_id=_id, text=templates.msg_start(first_name=_first_name))
            pass
        elif _text == "/list":
            send_message(chat_id=_id, text=templates.msg_list(),
                         keyboard_layout=generate_keyboard(settings.TelegramAPI.list_parse))
            pass
        elif _text == "/help":
            send_message(chat_id=_id, text=templates.msg_help())
            pass
        elif _text.startswith("+"):
            # send_message(chat_id=_id, text=templates.msg_added_to_list())
            add_element_to_list(element_name=_text[1:], chat_id=_id, user_id=_id)
            pass
    except AttributeError as error:
        logger("ERROR", "events_processing", f"Can't properly handle incoming event. [{error}]")
        pass


def send_message(text, chat_id, keyboard_layout=None):
    if text is None or chat_id is None:
        logger("ERROR", "send_message()", "'text' or 'chat_id' is None.")
        return

    return get_json_from_url(f"{telegram_base_url}/sendMessage?"
                             f"chat_id={chat_id}&"
                             f"text={quote(text)}&" +
                             (f"reply_markup={quote(keyboard_layout)}" if keyboard_layout is not None else ""))


def generate_keyboard(list_array):
    keyboard_layout = {
        "inline_keyboard": [

        ]
    }
    for element in list_array:
        keyboard_layout["inline_keyboard"].insert(0, [{"text": f"{element}", "callback_data": f"{uuid.uuid4()}"}])

    return json.dumps(keyboard_layout)
