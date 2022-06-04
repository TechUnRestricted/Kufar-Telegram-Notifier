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
#include "json.hpp"

namespace Telegram {
    using namespace std;
    using namespace Networking;
    using nlohmann::json;

    const short int MAX_IMAGES_IN_GROUP = 10;
    string makeImageGroupJSON(const vector<string> &images, const string &caption) {
        json j_array = json::array();
        for (int i = 0; (i < images.size()) && (i < MAX_IMAGES_IN_GROUP); i++){
            json j_list = json::object({
                    {"type", "photo"},
                    {"media", images[i]}
            });
            if (i == 0) {
                j_list.push_back({"caption", caption});
            }
            j_array.push_back(j_list);
        }
        
        return j_array.dump();
    }

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
                     "/sendMediaGroup?chat_id=" + telegramConfiguration.chatID_or_Username + "&"
                     "media=" + urlEncode(makeImageGroupJSON(ad.images, text));
        
        getJSONFromURL(url);
    }
};
