//
//  main.cpp
//  Kufar Telegram Notifier
//
//  Created by Macintosh on 02.06.2022.
//

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <vector>

#include "json.hpp"

#include "networking.hpp"
#include "kufar.hpp"
#include "telegram.hpp"

using namespace std;
using namespace Kufar;
using namespace Telegram;
using nlohmann::json;

bool vectorContains(const vector<int> &vector, const int &value) {
    if (find(vector.begin(), vector.end(), value) != vector.end()){
        return true;
    }
    return false;
}

struct ProgramConfiguration {
    vector<KufarConfiguration> kufarConfiguration;
    TelegramConfiguration telegramConfiguration;
    
    int queryDelaySeconds = 5;
    int loopDelaySeconds = 30;
};

string getTextFromFile(const string &path){
    /**
     Чтение файла и возврат
     содержимого в виде string.
     */
    
    ifstream ifs(path);
    return string((istreambuf_iterator<char>(ifs)),
                  (istreambuf_iterator<char>()));
}

void loadJSONConfigurationData(const json &data, ProgramConfiguration &programConfiguration){
    {
        json telegramData = data.at("telegram");
        programConfiguration.telegramConfiguration.botToken = telegramData.at("bot-token");
        programConfiguration.telegramConfiguration.chatID_or_Username = telegramData.at("chat-id");
    }
    {
        json queriesData = data.at("queries");
        for (const json &query : queriesData){
            KufarConfiguration kufarConfiguration;
            kufarConfiguration.tag = query.at("tag");
            kufarConfiguration.onlyTitleSearch = query.at("only-title-search");
            
            json queryPriceData = query.at("price");
            kufarConfiguration.priceMin = queryPriceData.at("min");
            kufarConfiguration.priceMax = queryPriceData.at("max");
            
            kufarConfiguration.language = query.at("language");
            kufarConfiguration.limit = query.at("limit");
            kufarConfiguration.region = query.at("region");
            kufarConfiguration.areas = (vector<int>)query.at("areas");
            programConfiguration.kufarConfiguration.push_back(kufarConfiguration);
        }
    }
    {
        json delaysData = data.at("delays");
        programConfiguration.queryDelaySeconds = delaysData.at("query");
        programConfiguration.loopDelaySeconds = delaysData.at("loop");
    }
}

void printJSONConfigurationData(const ProgramConfiguration &programConfiguration){
    cout <<
    "- Telegram:\n"
    "   - Token: " << programConfiguration.telegramConfiguration.botToken << "\n"
    "   - Chat ID: " << programConfiguration.telegramConfiguration.chatID_or_Username << "\n\n"
    "- Queries:\n\n";
    
    for (const auto &query : programConfiguration.kufarConfiguration){
        cout <<
        "   - Tag: " << query.tag << "\n"
        "   - Only Title Search: " << query.onlyTitleSearch << "\n"
        "   - Price:\n"
        "       - Min: " << query.priceMin.value() << "\n"
        "       - Max: " << query.priceMax.value() << "\n"
        "   - Language: " << query.language << "\n"
        "   - Limit: " << query.limit << "\n"
        "   - Region: " << (int)query.region << "\n"
        "   - Areas: ";
        
        for (const auto &area : query.areas){
            cout << area << ' ';
        }
        cout << "\n\n";
    }
    
    cout <<
    "- Delays:\n"
    "   - Query: " << programConfiguration.queryDelaySeconds << "\n"
    "   - Loop: " << programConfiguration.loopDelaySeconds << "\n";
}

int main() {
    string JSONPath = "/Users/macintosh/kufar-settings.json";
    cout << "[Loading configuration]: " << '"' << JSONPath << '"' << endl;
    
    ProgramConfiguration programConfiguration;
    json data = json::parse(getTextFromFile(JSONPath));
    loadJSONConfigurationData(data, programConfiguration);
    printJSONConfigurationData(programConfiguration);
    
    vector<int> viewedAds;
    while (true) {
        for (auto requestConfiguration : programConfiguration.kufarConfiguration){
            try {
                for (const auto &advert : getAds(requestConfiguration)){
                    if (!vectorContains(viewedAds, advert.id)){
                        cout << "[New]: Adding [Title: " << advert.title << "], [ID: " << advert.id << "], [Tag: " << advert.tag << "], [Link: " <<     advert.link << "]" << endl;
                        viewedAds.push_back(advert.id);
                        try {
                            sendAdvert(programConfiguration.telegramConfiguration, advert);
                        } catch (const exception &exc) {
                            cerr << "[ERROR (sendAdvert)]: " << exc.what() << endl;
                        }
                    } else {
                        //cout << "[Already was!]" << endl;
                    }
                    usleep(300000); // 0.3s
                }
            } catch (const exception &exc) {
                cerr << "[ERROR (getAds)]: " << exc.what() << endl;
            }
            cout << "[DEBUG]: " << "(QueryDelay) Sleeping for: " << programConfiguration.queryDelaySeconds << "s." << endl;
            sleep(programConfiguration.queryDelaySeconds);
        }
        cout << "[DEBUG]: " << "(LoopDelay) Sleeping for: " << programConfiguration.loopDelaySeconds << "s." << endl;
        sleep(programConfiguration.loopDelaySeconds);
    }
    return 0;
}
