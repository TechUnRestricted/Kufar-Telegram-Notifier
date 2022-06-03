//
//  telegram.hpp
//  Kufar Telegram Notifier
//
//  Created by Macintosh on 04.06.2022.
//

#ifndef Header_hpp
#define Header_hpp

namespace Telegram {
    using std::string;

    struct TelegramConfiguration {
        string botToken;
        string chatID_or_Username;
    };

    void sendAdvert(const TelegramConfiguration &, const Kufar::Ad &);
};
#endif /* Header_hpp */
