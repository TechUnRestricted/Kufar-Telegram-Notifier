//
//  networking.hpp
//  Kufar Telegram Notifier
//
//  Created by Macintosh on 04.06.2022.
//

#ifndef networking_hpp
#define networking_hpp

namespace Networking {
    std::string urlEncode(const std::string &);
    std::string getJSONFromURL(const std::string &);
};

#endif /* networking_hpp */
