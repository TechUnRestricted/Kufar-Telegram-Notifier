//
//  helperfunctions.hpp
//  Kufar Telegram Notifier
//
//  Created by Macintosh on 04.06.2022.
//

#ifndef helperfunctions_hpp
#define helperfunctions_hpp

#ifdef DEBUG
    #define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
    #define DEBUG_MSG(str) do { } while ( false )
#endif

#include <optional>
#include "json.hpp"

static const std::string PROPERTY_UNDEFINED = "[UNDEFINED]";

template<typename T>
std::optional<T> get_at_optional(const nlohmann::json &obj, const std::string &key) try {
    return obj.at(key).get<T>();
} catch (...) {
    return std::nullopt;
}

template<typename T>
std::ostream &operator << (std::ostream &os, std::optional<T> const &opt){
    return opt ? (os << opt.value()) : (os << PROPERTY_UNDEFINED);
}

bool vectorContains(const std::vector<int> &, const int &);
std::string getTextFromFile(const std::string &);
time_t zuluToTimestamp(const std::string &);
std::string joinIntVector(const std::vector<int> &, const std::string &);
time_t timestampShift(const time_t &, int);

#endif /* helperfunctions_hpp */
