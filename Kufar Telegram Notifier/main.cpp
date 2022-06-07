//
//  main.cpp
//  Kufar Telegram Notifier
//
//  Created by Macintosh on 02.06.2022.
//

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <vector>

#include "json.hpp"
#include "kufar.hpp"
#include "telegram.hpp"
#include "networking.hpp"
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
        programConfiguration.telegramConfiguration.chatID = telegramData.at("chat-id");
    }
    {
        json queriesData = data.at("queries");
        
        unsigned int index = 0;
        for (const json &query : queriesData){
            KufarConfiguration kufarConfiguration;
            
            if (query.contains("tag")) {
                kufarConfiguration.tag = query.at("tag");
            } else {
                cout << "[CRITICAL ERROR]: Tag field is empty at [" << index << "] position." << endl;
                exit(1);
            }
            
            kufarConfiguration.onlyTitleSearch = get_at_optional<bool>(query, "only-title-search");
            
            if (query.contains("price")){
                json queryPriceData = query.at("price");
                kufarConfiguration.priceRange.priceMin = get_at_optional<int>(queryPriceData, "min");
                kufarConfiguration.priceRange.priceMax = get_at_optional<int>(queryPriceData, "max");
            }
            
            kufarConfiguration.language = get_at_optional<string>(query, "language");
            kufarConfiguration.limit = get_at_optional<int>(query, "limit");
            kufarConfiguration.region = get_at_optional<int>(query, "region");
            kufarConfiguration.areas = get_at_optional<vector<int>>(query, "areas");
            programConfiguration.kufarConfiguration.push_back(kufarConfiguration);
            
            index += 1;
        }
    }
    {
        if (data.contains("delays")) {
            json delaysData = data.at("delays");
            programConfiguration.queryDelaySeconds = delaysData.at("query");
            programConfiguration.loopDelaySeconds = delaysData.at("loop");
        }
    }
}

void printJSONConfigurationData(const ProgramConfiguration &programConfiguration){
    cout <<
    "- Telegram:\n"
        "\t- Token: " << programConfiguration.telegramConfiguration.botToken << "\n"
        "\t- Chat ID: " << programConfiguration.telegramConfiguration.chatID << "\n\n"
    "- Queries:\n\n";
    
    for (const auto &query : programConfiguration.kufarConfiguration){
        cout <<
        "\t- Tag: " << query.tag << "\n"
        "\t- Only Title Search: " << query.onlyTitleSearch << "\n"
        "\t- Price:\n"
            "\t\t- Min: " << query.priceRange.priceMin << " BYN\n"
            "\t\t- Max: " << query.priceRange.priceMax << " BYN\n"
        "\t- Language: " << query.language << "\n"
        "\t- Limit: " << query.limit << "\n"
        "\t- Region: " << query.region << "\n"
        "\t- Areas: ";
        
        for (const auto &area : *query.areas){
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
            DEBUG_MSG("[DEBUG]: " << "(QueryDelay) Sleeping for: " << programConfiguration.queryDelaySeconds << "s.");
            sleep(programConfiguration.queryDelaySeconds);
        }
        DEBUG_MSG("[DEBUG]: " << "(LoopDelay) Sleeping for: " << programConfiguration.loopDelaySeconds << "s.");
        sleep(programConfiguration.loopDelaySeconds);
    }
    return 0;
}
