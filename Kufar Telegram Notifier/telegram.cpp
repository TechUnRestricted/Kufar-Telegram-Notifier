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
        string formattedTime = ctime(&ad.date);
        formattedTime.pop_back();
        
        string text =
            "#" + ad.tag + "\n"
            "Title: " + ad.title + "\n"
            "Date: " + formattedTime + "\n"
            "Price: " + to_string(ad.price / 100) + " BYN\n\n"
            "Seller Name: " + ad.sellerName + "\n"
            "Phone number is visible: " + (ad.phoneNumberIsVisible ? "Yes" : "No") + "\n"
            "Link: " + ad.link;
        
        string url = "https://api.telegram.org/bot" + telegramConfiguration.botToken;
        if (!ad.images.empty()){
            url += "/sendMediaGroup?chat_id=" + to_string(telegramConfiguration.chatID) + "&"
                   "media=" + urlEncode(makeImageGroupJSON(ad.images, text));
        } else {
            url += "/sendPhoto?chat_id=" + to_string(telegramConfiguration.chatID) + "&"
                   "caption=" + urlEncode(text) + "&"
                   "photo=https://via.placeholder.com/1080";
            /*url += "/sendMessage?chat_id=" + telegramConfiguration.chatID_or_Username + "&"
                   "text=" + urlEncode("[No Photo]\n\n" + text);*/
        }
        
        getJSONFromURL(url);
    }
};
