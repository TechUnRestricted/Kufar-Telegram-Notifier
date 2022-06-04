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
#include "helperfunctions.hpp"

using namespace std;
using namespace Kufar;
using namespace Telegram;
using nlohmann::json;



struct ProgramConfiguration {
    vector<KufarConfiguration> kufarConfiguration;
    TelegramConfiguration telegramConfiguration;
    
    int queryDelaySeconds = 5;
    int loopDelaySeconds = 30;
};

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
        "\t- Token: " << programConfiguration.telegramConfiguration.botToken << "\n"
        "\t- Chat ID: " << programConfiguration.telegramConfiguration.chatID_or_Username << "\n\n"
    "- Queries:\n\n";
    
    for (const auto &query : programConfiguration.kufarConfiguration){
        cout <<
        "\t- Tag: " << query.tag << "\n"
        "\t- Only Title Search: " << query.onlyTitleSearch << "\n"
        "\t- Price:\n"
            "\t\t- Min: " << query.priceMin.value() << "\n"
            "\t\t- Max: " << query.priceMax.value() << "\n"
        "\t- Language: " << query.language << "\n"
        "\t- Limit: " << query.limit << "\n"
        "\t- Region: " << (int)query.region << "\n"
        "\t- Areas: ";
        
        for (const auto &area : query.areas){
            cout << area << ' ';
        }
        cout << "\n\n";
    }
    
    cout <<
    "- Delays:\n"
    "   - Query: " << programConfiguration.queryDelaySeconds << "\n"
    "   - Loop: " << programConfiguration.loopDelaySeconds << endl;
}

int main(int argc, char *argv[]) {
    if (argc < 2){
        cerr << "[ERROR]: No JSON file path passed!" << endl;
        exit(1);
    }
    
    string JSONPath = argv[1];
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
            DEBUG_MSG("[DEBUG]: " << "(QueryDelay) Sleeping for: " << programConfiguration.queryDelaySeconds << "s." << endl);
            sleep(programConfiguration.queryDelaySeconds);
        }
        DEBUG_MSG("[DEBUG]: " << "(LoopDelay) Sleeping for: " << programConfiguration.loopDelaySeconds << "s." << endl);
        sleep(programConfiguration.loopDelaySeconds);
    }
    return 0;
}
