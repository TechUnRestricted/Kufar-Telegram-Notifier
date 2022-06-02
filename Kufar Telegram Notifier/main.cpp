//
//  main.cpp
//  Kufar Telegram Notifier
//
//  Created by Macintosh on 02.06.2022.
//

#include <iostream>
#include <unistd.h>

#include "json.hpp"
#include "networking.hpp"
#include "kufar.hpp"

using namespace std;
using namespace Kufar;

int main() {
    Query query;
    query.tag = "iPhone";
    query.priceMax = 80 * 100;
    query.priceMin = 0;
    
    query.onlyTitleSearch = true;
    Configuration configuration;
    configuration.limit = 5;
    configuration.language = "ru";
    configuration.region = Region::Minsk;

    configuration.areas = {
        (int)Areas::Minsk::Pervomajskij,
        (int)Areas::Minsk::Moskovskij
    };
    
    configuration
        .queries
        .push_back(query);
    
    
    
    auto adverts = getAds(configuration);
    
    return 0;
}
