//
//  networking.h
//  FromHateToLove
//
//  Created by Macintosh on 02.06.2022.
//

#ifndef networking_hpp
#define networking_hpp

namespace Networking {
    using std::string;

    string urlEncode(const std::string &);
    string getJSONFromURL(const std::string &);
};

#endif /* networking_hpp */
