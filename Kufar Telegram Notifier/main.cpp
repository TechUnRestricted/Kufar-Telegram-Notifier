//
//  main.cpp
//  Kufar Telegram Notifier
//
//  Created by Macintosh on 02.06.2022.
//

#include <iostream>
#include <unistd.h>
#include <vector>

//#include "json.hpp"
#include "networking.hpp"
#include "kufar.hpp"
#include "telegram.hpp"

using namespace std;
using namespace Kufar;
using namespace Telegram;

bool vectorContains(const vector<int> &vector, const int &value) {
    if (find(vector.begin(), vector.end(), value) != vector.end()){
        return true;
    }
    return false;
}

struct ProgramConfiguration {
    vector<KufarConfiguration> requestConfiguration;

    int queryDelaySeconds = 5;
    int loopDelaySeconds = 30;
};


int main() {
    ProgramConfiguration programConfiguration;
    TelegramConfiguration telegramConfiguration;
    telegramConfiguration.chatID_or_Username = "0000000000";
    telegramConfiguration.botToken = "0000000000:AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    
    {
    KufarConfiguration kufarConfiguration;
    
    kufarConfiguration.limit = 5;
    kufarConfiguration.language = "ru";
    kufarConfiguration.region = Region::Minsk;
    kufarConfiguration.tag = "iPhone";
    kufarConfiguration.priceMax = 80 * 100;
    kufarConfiguration.priceMin = 0;
    kufarConfiguration.onlyTitleSearch = false;
    
    kufarConfiguration.areas = {
        (int)Areas::Minsk::Pervomajskij,
        (int)Areas::Minsk::Moskovskij
    };
    
    programConfiguration.requestConfiguration.push_back(kufarConfiguration);
    kufarConfiguration.tag = "iPad";
    programConfiguration.requestConfiguration.push_back(kufarConfiguration);
        
    }
    
    vector<int> viewedAds;
    while (true) {
        for (auto requestConfiguration : programConfiguration.requestConfiguration){
            for (const auto &advert : getAds(requestConfiguration)){
                if (!vectorContains(viewedAds, advert.id)){
                   viewedAds.push_back(advert.id);
                   sendAdvert(telegramConfiguration, advert);
                   usleep(300000); // 0.3s
                } else {
                    cout << "[Already was!]" << endl;
                }
            }
            cout << "[DEBUG]: " << "(QueryDelay) Sleeping for: " << programConfiguration.queryDelaySeconds << "s." << endl;
            sleep(programConfiguration.queryDelaySeconds);
        }
        cout << "[DEBUG]: " << "(LoopDelay) Sleeping for: " << programConfiguration.loopDelaySeconds << "s." << endl;
        sleep(programConfiguration.loopDelaySeconds);
    }
    return 0;
}
