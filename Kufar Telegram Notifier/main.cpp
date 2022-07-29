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
            
            if (query.contains("tag")) {
                kufarConfiguration.tag = query.at("tag");
            } else {
                cout << "[КРИТИЧЕСКАЯ ОШИБКА]: Отсутствует название для поиска объявлений (tag) на позиции [" << index << "]." << endl;
                exit(1);
            }
            
            kufarConfiguration.onlyTitleSearch = getOptionalValue<bool>(query, "only-title-search");
            
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
    "   - Перед новым запросом: " << programConfiguration.queryDelaySeconds << "с. \n"
    "   - После прохода всего списка запросов: " << programConfiguration.loopDelaySeconds << "c." << endl;
}

int main(int argc, char *argv[]) {    
    if (argc < 2){
        cerr << "[ОШИБКА]: JSON Файл конфигурации не был передан в аргументы программы!" << endl;
        exit(1);
    }
    
    string JSONPath = argv[1];
    cout << "[Загрузка файла конфигурации]: " << '"' << JSONPath << '"' << endl;
    
    if (!fileExists(JSONPath)){
        cout << "[ОШИБКА]: Файл не существует по данному пути или к нему нет доступа." << endl;
        exit(1);
    }
    
    if (getFileSize(JSONPath) > 4000000) {
        cout << "[ОШИБКА]: Размер файла превышает 4МБ." << endl;
        exit(1);
    }
    
    ProgramConfiguration programConfiguration;
    json data = json::parse(getTextFromFile(JSONPath));
    loadJSONConfigurationData(data, programConfiguration);
    printJSONConfigurationData(programConfiguration);
    vector<int> viewedAds;
    while (true) {
        for (auto requestConfiguration : programConfiguration.kufarConfiguration) {
            try {
                for (const auto &advert : getAds(requestConfiguration)) {
                    if (!vectorContains(viewedAds, advert.id)) {
                        cout << "[New]: Adding [Title: " << advert.title << "], [ID: " << advert.id << "], [Tag: " << advert.tag << "], [Link: " << advert.link << "]" << endl;
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
