//
//  main.cpp
//  Kufar Telegram Notifier
//
//  Created by Macintosh on 02.06.2022.
//

#include <iostream>
#include <unistd.h>
#include <signal.h>
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

const string CACHE_FILE_NAME = "cached-data.json";
const string CONFIGURATION_FILE_NAME = "kufar-configuration.json";

struct ConfigurationFile {
    string path;
    json contents;
};

struct CacheFile {
    string path;
    json contents;
};

struct Files {
    ConfigurationFile configuration;
    CacheFile cache;
};

struct ProgramConfiguration {
    vector<KufarConfiguration> kufarConfiguration;
    TelegramConfiguration telegramConfiguration;
    Files files;
    
    int queryDelaySeconds = 5;
    int loopDelaySeconds = 30;
};

void loadJSONConfigurationData(const json &data, ProgramConfiguration &programConfiguration) {
    {
        json telegramData = data.at("telegram");
        programConfiguration.telegramConfiguration.botToken = telegramData.at("bot-token");
        programConfiguration.telegramConfiguration.chatID = telegramData.at("chat-id");
    }
    {
        json queriesData = data.at("queries");
        
        unsigned int index = 0;
        for (const json &query : queriesData) {
            KufarConfiguration kufarConfiguration;
            
            kufarConfiguration.onlyTitleSearch = getOptionalValue<bool>(query, "only-title-search");
            kufarConfiguration.tag = getOptionalValue<string>(query, "tag");
            if (query.contains("price")) {
                json queryPriceData = query.at("price");
                kufarConfiguration.priceRange.priceMin = getOptionalValue<int>(queryPriceData, "min");
                kufarConfiguration.priceRange.priceMax = getOptionalValue<int>(queryPriceData, "max");
            }
            
            kufarConfiguration.language = getOptionalValue<string>(query, "language");
            kufarConfiguration.limit = getOptionalValue<int>(query, "limit");
            kufarConfiguration.currency = getOptionalValue<string>(query, "currency");
            kufarConfiguration.condition = getOptionalValue<ItemCondition>(query, "condition");
            kufarConfiguration.sellerType = getOptionalValue<SellerType>(query, "seller-type");
            kufarConfiguration.kufarDeliveryRequired = getOptionalValue<bool>(query, "kufar-delivery-required");
            kufarConfiguration.kufarPaymentRequired = getOptionalValue<bool>(query, "kufar-payment-required");
            kufarConfiguration.kufarHalvaRequired = getOptionalValue<bool>(query, "kufar-halva-required");
            kufarConfiguration.onlyWithPhotos = getOptionalValue<bool>(query, "only-with-photos");
            kufarConfiguration.onlyWithVideos = getOptionalValue<bool>(query, "only-with-videos");
            kufarConfiguration.onlyWithExchangeAvailable = getOptionalValue<bool>(query, "only-with-exchange-available");
            kufarConfiguration.sortType = getOptionalValue<SortType>(query, "sort-type");
            kufarConfiguration.category = getOptionalValue<int>(query, "category");
            kufarConfiguration.region = getOptionalValue<Region>(query, "region");
            kufarConfiguration.areas = getOptionalValue<vector<int>>(query, "areas");
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

void printJSONConfigurationData(const ProgramConfiguration &programConfiguration) {
    cout <<
    "- Telegram:\n"
        "\t- Токен: " << programConfiguration.telegramConfiguration.botToken << "\n"
        "\t- ID Чата: " << programConfiguration.telegramConfiguration.chatID << "\n\n"
    "- Запросы:\n";
    
    for (const auto &query : programConfiguration.kufarConfiguration) {
        cout <<
        "\t- Название: " << query.tag << "\n"
        "\t- Поиск только по заголовку: " << query.onlyTitleSearch << "\n"
        "\t- Цена:\n"
            "\t\t- Минимальная: " << query.priceRange.priceMin << " BYN\n"
            "\t\t- Максимальная: " << query.priceRange.priceMax << " BYN\n"
        "\t- Язык: " << query.language << "\n"
        "\t- Макс. кол-во объявлений за один запрос: " << query.limit << "\n"
        "\t- Валюта: " << query.currency << "\n"
        
        "\t- Состояние: " << (query.condition.has_value() ?
             EnumString::itemCondition(query.condition.value()) : PROPERTY_UNDEFINED) << "\n"
        
        "\t- Продавец: " << (query.sellerType.has_value() ?
             EnumString::sellerType(query.sellerType.value()) : PROPERTY_UNDEFINED) << "\n"
        
        "\t- Только с Kufar Доставкой: " << query.kufarDeliveryRequired << "\n"
        "\t- Только с Kufar Оплатой: " << query.kufarPaymentRequired << "\n"
        
        
        "\t- Только с Kufar Рассрочкой (Халва): " << query.kufarPaymentRequired << "\n"
        "\t- Только с фото: " << query.onlyWithPhotos << "\n"
        "\t- Только с видео: " << query.onlyWithVideos << "\n"
        "\t- Только с возможностью обмена: " << query.onlyWithExchangeAvailable << "\n"
        "\t- Тип сортировки: " << (query.sortType.has_value() ? EnumString::sortType(query.sortType.value()) : PROPERTY_UNDEFINED) << "\n"
        "\t- Категория: " << query.category << "\n"

        "\t- Город: " << (query.region.has_value() ? EnumString::region(query.region.value()) : PROPERTY_UNDEFINED)<< "\n"
        
        "\t- Район: ";
        
        if (query.areas.has_value()) {
            unsigned int currentIndex = 0;
            const uint64_t &vectorSize = query.areas.value().size();
            
            for (const auto &area : query.areas.value()){
                cout << EnumString::area(area);
                
                if (currentIndex++ < vectorSize - 1) {
                    cout << ", ";
                }
            }
        } else {
            cout << PROPERTY_UNDEFINED;
        }
        
        cout << "\n\n";
    }
    
    cout <<
    "- Задержки:\n"
        "\t- Перед новым запросом: " << programConfiguration.queryDelaySeconds << "с. \n"
        "\t- После прохода всего списка запросов: " << programConfiguration.loopDelaySeconds << "c." << endl;
}

json getJSONDataFromPath(const string &JSONFilePath) {
    cout << "[Загрузка файла]: " << '"' << JSONFilePath << '"' << endl;

    if (!fileExists(JSONFilePath)){
        cout << "[ОШИБКА]: Файл не существует по данному пути или к нему нет доступа." << endl;
        exit(1);
    }
    
    if (getFileSize(JSONFilePath) > 4000000) {
        cout << "[ОШИБКА]: Размер файла превышает 4МБ." << endl;
        exit(1);
    }
        
    try {
       json textFromFile = json::parse(getTextFromFile(JSONFilePath));
       return textFromFile;
    } catch (const exception &exc) {
       cout << "[ОШИБКА]: Невозможно получить данные из файла " << '"' << JSONFilePath << '"' << endl;
       cout << "::: " << exc.what() << " :::" << endl;
       exit(1);
    }

}

const string prefixConfigurationFile = "--config=";
const string prefixCacheFile = "--cache=";

/**
  Загрузка файлов:
  kufar-configuration.json,
  cached-data.json
 */

Files getFiles(const int &argsCount, char **args) {
    Files files;
    
    for (int i = 0; i < argsCount; i++){
        string currentArgument = args[i];
        
        if(stringHasPrefix(currentArgument, prefixConfigurationFile)) {
            currentArgument.erase(0, prefixConfigurationFile.length());
            files.configuration.path = currentArgument;
        } else if (stringHasPrefix(currentArgument, prefixCacheFile)) {
            currentArgument.erase(0, prefixCacheFile.length());
            files.cache.path = currentArgument;
        }
        
    }
    
    if (files.configuration.path.empty() || files.cache.path.empty()) {
        optional<string> applicationDirectory = getWorkingDirectory();
        
        if (!applicationDirectory.has_value()) {
            cout << "[ОШИБКА]: Невозможно автоматически определить путь к текущей папке. Передайте файл конфигурации/кеша в виде аргумента." << endl;
            exit(1);
        }
        
        if (files.configuration.path.empty()) {
            files.configuration.path = applicationDirectory.value() + PATH_SEPARATOR + CONFIGURATION_FILE_NAME;
        }
        
        if (files.cache.path.empty()) {
            files.cache.path = applicationDirectory.value() + PATH_SEPARATOR + CACHE_FILE_NAME;
        }
    }
    
    files.configuration.contents = getJSONDataFromPath(files.configuration.path);
    files.cache.contents = getJSONDataFromPath(files.cache.path);
    
    return files;
}

int main(int argc, char **argv) {
    ProgramConfiguration programConfiguration;
    vector<int> viewedAds;
    
    programConfiguration.files = getFiles(argc, argv);
    loadJSONConfigurationData(programConfiguration.files.configuration.contents, programConfiguration);
    printJSONConfigurationData(programConfiguration);

    viewedAds = programConfiguration.files.cache.contents.get<vector<int>>();

    while (true) {
        for (auto requestConfiguration : programConfiguration.kufarConfiguration) {
            unsigned int sentCount = 0;
            
            try {
                for (const auto &advert : getAds(requestConfiguration)) {
                    if (!vectorContains(viewedAds, advert.id)) {
                        cout << "[New]: Adding [Title: " << advert.title << "], [ID: " << advert.id << "], [Tag: " << advert.tag << "], [Link: " << advert.link << "]" << endl;
                        viewedAds.push_back(advert.id);
                        sentCount += 1;

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
            
            if (sentCount > 0) {
                saveFile(programConfiguration.files.cache.path, ((json)viewedAds).dump());
            }
        }
        DEBUG_MSG("[DEBUG]: " << "(LoopDelay) Sleeping for: " << programConfiguration.loopDelaySeconds << "s.");
        sleep(programConfiguration.loopDelaySeconds);
    }
    return 0;
}
