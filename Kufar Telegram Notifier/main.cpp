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

bool vectorContains(const vector<int> &vector, const int &value) {
    if (find(vector.begin(), vector.end(), value) != vector.end()){
        return true;
    }
    return false;
}

struct ProgramConfiguration {
    vector<KufarConfiguration> requestConfiguration;

    int queryDelaySeconds = 10;
    int loopDelaySeconds = 30;
};


int main() {
    KufarConfiguration kufarConfiguration;
    
    kufarConfiguration.limit = 5;
    kufarConfiguration.language = "ru";
    kufarConfiguration.region = Region::Minsk;
    kufarConfiguration.tag = "Носок";
    kufarConfiguration.priceMax = 80 * 100;
    kufarConfiguration.priceMin = 0;
    kufarConfiguration.onlyTitleSearch = false;
    
    kufarConfiguration.areas = {
        (int)Areas::Minsk::Pervomajskij,
        (int)Areas::Minsk::Moskovskij
    };
    
    ProgramConfiguration programConfiguration;
    programConfiguration.requestConfiguration.push_back(kufarConfiguration);
    
    vector<int> viewedAds;
    while (true) {
        auto adverts = getAds(kufarConfiguration);
        for (auto advert : adverts){
            if (!vectorContains(viewedAds, advert.id)){
                viewedAds.push_back(advert.id);
            } else {
                cout << "[Already was!]" << endl;
            }
        }
        sleep(programConfiguration.loopDelaySeconds);
    }
    return 0;
}
