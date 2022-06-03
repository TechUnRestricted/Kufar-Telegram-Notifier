//
//  telegram.cpp
//  Kufar Telegram Notifier
//
//  Created by Macintosh on 04.06.2022.
//

#include <iostream>
#include "kufar.hpp"
#include "telegram.hpp"
#include "networking.hpp"

namespace Telegram {
    using std::string;
    using namespace Networking;

    void sendAdvert(const TelegramConfiguration &telegramConfiguration, const Kufar::Ad &ad) {
        string text =
        "#" + ad.tag + "\n"
        "Title: " + ad.title + "\n"
        "Date: " + std::to_string(ad.date) + "\n"
        "Price: " + std::to_string(ad.price / 100) + " BYN\n"
        "Seller Name: " + ad.sellerName + "\n"
        "Phone number is visible: " + (ad.phoneNumberIsVisible ? "Yes" : "No") + "\n"
        "Link: " + ad.link;
        
        string url = "https://api.telegram.org/bot" +
                     telegramConfiguration.botToken +
                     "/sendPhoto?caption=" + urlEncode(text) + "&"
                     "chat_id=" + telegramConfiguration.chatID_or_Username + "&"
                     "photo=" + ad.images.front();
        
        getJSONFromURL(url);
    }
};
