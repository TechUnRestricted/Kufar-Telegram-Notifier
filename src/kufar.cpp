//
//  kufar.cpp
//  Kufar Telegram Notifier
//
//  Created by Macintosh on 04.06.2022.
//

#include "json.hpp"
#include "kufar.hpp"
#include "networking.hpp"
#include "helperfunctions.hpp"
#include <iostream>

namespace Kufar {

    using namespace std;
    using namespace Networking;
    using nlohmann::json;

    const string baseURL = "https://searchapi.kufar.by/v1/search/rendered-paginated?";
    const string DEFAULT_MAX_PRICE = "1000000000";

    optional<string> PriceRange::joinPrice() const {
        if (!priceMin.has_value() && !priceMax.has_value()) { return nullopt; }
        
        string joinedPrice = "";
        
        if (!priceMin.has_value()) {
            joinedPrice += '0';
        } else {
            joinedPrice += to_string(priceMin.value() * 100);
        }
        
        joinedPrice = "r:" + joinedPrice + ',' + (priceMax.has_value() ? to_string(priceMax.value() * 100) : DEFAULT_MAX_PRICE);
        
        return joinedPrice;
    }
    
    string getSortTypeUrlParameter(SortType sortType) {
        switch (sortType) {
            case SortType::descending:
                return "prc.d";
            case SortType::ascending:
                return "prc.a";
            default:
                // TODO: Передалать под возврат nullopt;
                return "";
        }
    }

    namespace {
        void insertImageURL (vector<string> &images, const string &id, const bool yams_storage) {
            if (yams_storage) {
                images.push_back("https://yams.kufar.by/api/v1/kufar-ads/images/" + id.substr(0, 2) + "/" + id + ".jpg?rule=pictures");
            }
        }
    
        void addURLParameter(ostringstream &ostream, const string &parameter, const string &value, const bool encodeValue = false) {
            ostream << parameter << '=' << (encodeValue ? urlEncode(value) : value) << '&';
        }
        
        void addURLParameter(ostringstream &ostream, const string &parameter, const optional<string> &value, const bool encodeValue = false) {
            if (value.has_value()) {
                addURLParameter(ostream, parameter, value.value(), encodeValue);
            }
        }
        
        void addURLParameterBoolean(ostringstream &ostream, const string &parameter, const optional<bool> &value, const bool encodeValue = false) {
            if (value.has_value() && value.value() == true) {
                addURLParameter(ostream, parameter, to_string(value.value()), encodeValue);
            }
        }
    
        void addURLParameter(ostringstream &ostream, const string &parameter, const optional<int> &value, const bool encodeValue = false) {
            if (value.has_value()) {
                addURLParameter(ostream, parameter, to_string(value.value()), encodeValue);
            }
        }
    
    }

    vector<Ad> getAds(const KufarConfiguration &configuration) {
        vector<Ad> adverts;
        ostringstream urlStream;
        urlStream << baseURL;
        
        addURLParameter(urlStream, "query", configuration.tag, true);
        addURLParameter(urlStream, "lang", configuration.language);
        addURLParameter(urlStream, "size", configuration.limit);
        addURLParameter(urlStream, "prc", configuration.priceRange.joinPrice());
        addURLParameter(urlStream, "cur", configuration.currency);
        addURLParameter(urlStream, "cat", configuration.subCategory);
        addURLParameter(urlStream, "prn", configuration.category);

        addURLParameterBoolean(urlStream, "ot", configuration.onlyTitleSearch);
        addURLParameterBoolean(urlStream, "dle", configuration.kufarDeliveryRequired);
        addURLParameterBoolean(urlStream, "sde", configuration.kufarPaymentRequired);
        addURLParameterBoolean(urlStream, "hlv", configuration.kufarHalvaRequired);
        addURLParameterBoolean(urlStream, "oph", configuration.onlyWithPhotos);
        addURLParameterBoolean(urlStream, "ovi", configuration.onlyWithVideos);
        addURLParameterBoolean(urlStream, "pse", configuration.onlyWithExchangeAvailable);
        
        if (configuration.sortType.has_value()) { addURLParameter(urlStream, "sort", getSortTypeUrlParameter(configuration.sortType.value())); }
        if (configuration.condition.has_value()) { addURLParameter(urlStream, "cnd", int(configuration.condition.value())); }
        if (configuration.sellerType.has_value()) { addURLParameterBoolean(urlStream, "cmp", int(configuration.sellerType.value())); }
        if (configuration.region.has_value()) { addURLParameter(urlStream, "rgn", int(configuration.region.value())); }
        if (configuration.areas.has_value()) { addURLParameter(urlStream, "ar", "v.or:" + joinIntVector(configuration.areas.value(), ",")); }
            
        string rawJson = getJSONFromURL(urlStream.str());
        
        json ads = json::parse(rawJson).at("ads");

        for (const auto &ad : ads) {
            Ad advert;
            
            if (configuration.tag.has_value()) {
                advert.tag = configuration.tag.value();
            }
            
            advert.title = ad.at("subject");
            advert.id = ad.at("ad_id");
            advert.date = timestampShift(zuluToTimestamp((string)ad.at("list_time")), 3);
            advert.price = stoi((string)ad.at("price_byn"));
            advert.phoneNumberIsVisible = !ad.at("phone_hidden");
            advert.link = ad.at("ad_link");
            
            json accountParameters = ad.at("account_parameters");
            for (const auto &accountParameter : accountParameters) {
                if (accountParameter.at("p") == "name") {
                    advert.sellerName = accountParameter.at("v");
                    break;
                }
            }
            
            json imagesArray = ad.at("images");
            for (const auto &image : imagesArray) {
                string imageID = image.at("id");
                bool isYams = image.at("yams_storage");
                insertImageURL(advert.images, imageID, isYams);
            }
            
            adverts.push_back(advert);
        }
        
        return adverts;
    }

    namespace EnumString {
        string sortType(SortType sortType) {
            switch (sortType) {
                case SortType::descending:
                    return "По убыванию";
                case SortType::ascending:
                    return "По возрастанию";
                default:
                    return "[Неизвестный тип]";
            }
        }
        
        string itemCondition(ItemCondition itemCondition) {
            switch (itemCondition) {
                case ItemCondition::_new:
                    return "Новое";
                case ItemCondition::used:
                    return "Б/У";
                default:
                    return "[Неизвестное состояние]";
            }
        }
    
        string sellerType(SellerType sellerType) {
            switch (sellerType) {
                case SellerType::individualPerson:
                    return "Частное лицо";
                case SellerType::company:
                    return "Компания";
                default:
                    return "[Неизвестный тип]";
            }
        }
    
        string region(Region region) {
            switch (region) {
                case Region::Brest:
                    return "Брест";
                case Region::Gomel:
                    return "Гомель";
                case Region::Grodno:
                    return "Гродно";
                case Region::Mogilev:
                    return "Могилёв";
                case Region::Minsk_Region:
                    return "Минская область";
                case Region::Vitebsk:
                    return "Витебск";
                case Region::Minsk:
                    return "Минск";
                default:
                    return "[Неизвестный регион]";
            }
        }
    
        string area(int value) {
            switch (value) {
                ///@b Минск
                case int(Areas::Minsk::Centralnyj):
                    return "Центральный";
                case int(Areas::Minsk::Sovetskij):
                    return "Советский";
                case int(Areas::Minsk::Pervomajskij):
                    return "Первомайский";
                case int(Areas::Minsk::Partizanskij):
                    return "Партизанский";
                case int(Areas::Minsk::Zavodskoj):
                    return "Заводской";
                case int(Areas::Minsk::Leninskij):
                    return "Ленинский";
                case int(Areas::Minsk::Oktyabrskij):
                    return "Октябрьский";
                case int(Areas::Minsk::Moskovskij):
                    return "Московский";
                case int(Areas::Minsk::Frunzenskij):
                    return "Фрунзенский";
                 
                ///@b Брестская область
                case int(Areas::Brest::Brest):
                    return "Брест";
                case int(Areas::Brest::Baranovichi):
                    return "Барановичи";
                case int(Areas::Brest::Bereza):
                    return "Береза";
                case int(Areas::Brest::Beloozyorsk):
                    return "Белоозёрск";
                case int(Areas::Brest::Gancevichi):
                    return "Ганцевичи";
                case int(Areas::Brest::Drogichin):
                    return "Дрогичин";
                case int(Areas::Brest::Zhabinka):
                    return "Жабинка";
                case int(Areas::Brest::Ivanovo):
                    return "Иваново";
                case int(Areas::Brest::Ivacevichi):
                    return "Иванцевичи";
                case int(Areas::Brest::Kamenec):
                    return "Каменец";
                case int(Areas::Brest::Kobrin):
                    return "Кобрин";
                case int(Areas::Brest::Luninec):
                    return "Лунинец";
                case int(Areas::Brest::Lyahovichi):
                    return "Ляховичи";
                case int(Areas::Brest::Malorita):
                    return "Малорита";
                case int(Areas::Brest::Pinsk):
                    return "Пинск";
                case int(Areas::Brest::Pruzhany):
                    return "Пружаны";
                case int(Areas::Brest::Stolin):
                    return "Столин";
                case int(Areas::Brest::Others):
                    return "Другое (Брест)";
                 
                ///@b Гомельская область
                case int(Areas::Gomel::Gomel):
                    return "Гомель";
                case int(Areas::Gomel::Bragin):
                    return "Брагин";
                case int(Areas::Gomel::BudaKoshelevo):
                    return "Буда-Кошелёво";
                case int(Areas::Gomel::Vetka):
                    return "Ветка";
                case int(Areas::Gomel::Dobrush):
                    return "Добруш";
                case int(Areas::Gomel::Elsk):
                    return "Ельск";
                case int(Areas::Gomel::Zhitkovichi):
                    return "Житковичи";
                case int(Areas::Gomel::Zhlobin):
                    return "Жлобин";
                case int(Areas::Gomel::Kalinkovichi):
                    return "Калинковичи";
                case int(Areas::Gomel::Korma):
                    return "Корма";
                case int(Areas::Gomel::Lelchicy):
                    return "Лельчицы";
                case int(Areas::Gomel::Loev):
                    return "Лоев";
                case int(Areas::Gomel::Mozyr):
                    return "Мозырь";
                case int(Areas::Gomel::Oktyabrskij):
                    return "Октябрьский";
                case int(Areas::Gomel::Narovlya):
                    return "Наровля";
                case int(Areas::Gomel::Petrikov):
                    return "Петриков";
                case int(Areas::Gomel::Rechica):
                    return "Речица";
                case int(Areas::Gomel::Rogachev):
                    return "Рогачёв";
                case int(Areas::Gomel::Svetlogorsk):
                    return "Светлогорск";
                case int(Areas::Gomel::Hojniki):
                    return "Хойники";
                case int(Areas::Gomel::Chechersk):
                    return "Чечерск";
                case int(Areas::Gomel::Others):
                    return "Другое (Гомель)";
                
                ///@b Гродненская область
                case int(Areas::Grodno::Grodno):
                    return "Гродно";
                case int(Areas::Grodno::Berezovka):
                    return "Берёзовка";
                case int(Areas::Grodno::Berestovica):
                    return "Берестовица";
                case int(Areas::Grodno::Volkovysk):
                    return "Волковыск";
                case int(Areas::Grodno::Voronovo):
                    return "Вороново";
                case int(Areas::Grodno::Dyatlovo):
                    return "Дятлово";
                case int(Areas::Grodno::Zelva):
                    return "Зельва";
                case int(Areas::Grodno::Ive):
                    return "Ивье";
                case int(Areas::Grodno::Korelichi):
                    return "Кореличи";
                case int(Areas::Grodno::Lida):
                    return "Лида";
                case int(Areas::Grodno::Mosty):
                    return "Мосты";
                case int(Areas::Grodno::Novogrudok):
                    return "Новогрудок";
                case int(Areas::Grodno::Ostrovec):
                    return "Островец";
                case int(Areas::Grodno::Oshmyany):
                    return "Ошмяны";
                case int(Areas::Grodno::Svisloch):
                    return "Свислочь";
                case int(Areas::Grodno::Skidel):
                    return "Скидель";
                case int(Areas::Grodno::Slonim):
                    return "Слоним";
                case int(Areas::Grodno::Smorgon):
                    return "Сморгонь";
                case int(Areas::Grodno::Shchuchin):
                    return "Щучин";
                case int(Areas::Grodno::Others):
                    return "Другое (Гродно)";
                
                ///@b Могилёв
                case int(Areas::Mogilev::Mogilev):
                    return "Могилёв";
                case int(Areas::Mogilev::Belynichi):
                    return "Белыничи";
                case int(Areas::Mogilev::Bobrujsk):
                    return "Бобруйск";
                case int(Areas::Mogilev::Byhov):
                    return "Быхов";
                case int(Areas::Mogilev::Glusk):
                    return "Глуск";
                case int(Areas::Mogilev::Gorki):
                    return "Горки";
                case int(Areas::Mogilev::Dribin):
                    return "Дрибин";
                case int(Areas::Mogilev::Kirovsk):
                    return "Кировск";
                case int(Areas::Mogilev::Klimovichi):
                    return "Климовичи";
                case int(Areas::Mogilev::Klichev):
                    return "Кличев";
                case int(Areas::Mogilev::Mstislavl):
                    return "Мстиславль";
                case int(Areas::Mogilev::Osipovichi):
                    return "Осиповичи";
                case int(Areas::Mogilev::Slavgorod):
                    return "Славгород";
                case int(Areas::Mogilev::Chausy):
                    return "Чаусы";
                case int(Areas::Mogilev::Cherikov):
                    return "Чериков";
                case int(Areas::Mogilev::Shklov):
                    return "Шклов";
                case int(Areas::Mogilev::Hotimsk):
                    return "Хотимск";
                case int(Areas::Mogilev::Others):
                    return "Другое (Могилёв)";
                
               ///@b Минская область
               case int(Areas::MinskRegion::MinskRegion):
                   return "Минский район";
               case int(Areas::MinskRegion::Berezino):
                   return "Березино";
               case int(Areas::MinskRegion::Borisov):
                   return "Борисов";
               case int(Areas::MinskRegion::Vilejka):
                   return "Вилейка";
               case int(Areas::MinskRegion::Volozhin):
                   return "Воложин";
               case int(Areas::MinskRegion::Dzerzhinsk):
                   return "Дзержинск";
               case int(Areas::MinskRegion::Zhodino):
                   return "Жодино";
               case int(Areas::MinskRegion::Zaslavl):
                   return "Заславль";
               case int(Areas::MinskRegion::Kleck):
                   return "Клецк";
               case int(Areas::MinskRegion::Kopyl):
                   return "Копыль";
               case int(Areas::MinskRegion::Krupki):
                   return "Крупки";
               case int(Areas::MinskRegion::Logojsk):
                   return "Логойск";
               case int(Areas::MinskRegion::Lyuban):
                   return "Люба💔нь";
               case int(Areas::MinskRegion::MarinaGorka):
                   return "Марьина Горка";
               case int(Areas::MinskRegion::Molodechno):
                   return "Молодечно";
               case int(Areas::MinskRegion::Myadel):
                   return "Мядель";
               case int(Areas::MinskRegion::Nesvizh):
                   return "Несвиж";
               case int(Areas::MinskRegion::Rudensk):
                   return "Руденск";
               case int(Areas::MinskRegion::Sluck):
                   return "Слуцк";
               case int(Areas::MinskRegion::Smolevichi):
                   return "Смолевичи";
               case int(Areas::MinskRegion::Soligorsk):
                   return "Солигорск";
               case int(Areas::MinskRegion::StaryeDorogi):
                   return "Старые Дороги";
               case int(Areas::MinskRegion::Stolbcy):
                   return "Столбцы";
               case int(Areas::MinskRegion::Uzda):
                   return "Узда";
               case int(Areas::MinskRegion::Fanipol):
                   return "Фаниполь";
               case int(Areas::MinskRegion::Cherven):
                   return "Червень";
               case int(Areas::MinskRegion::Others):
                   return "Другое (Минская область)";
               
               ///@b Витебская область
               case int(Areas::Vitebsk::Vitebsk):
                   return "Витбеск";
               case int(Areas::Vitebsk::Beshenkovichi):
                   return "Бешенковичи";
               case int(Areas::Vitebsk::Baran):
                   return "Барань";
               case int(Areas::Vitebsk::Braslav):
                   return "Браслав";
               case int(Areas::Vitebsk::Verhnedvinsk):
                   return "Верхнедвинск";
               case int(Areas::Vitebsk::Glubokoe):
                   return "Глубокое";
               case int(Areas::Vitebsk::Gorodok):
                   return "Городок";
               case int(Areas::Vitebsk::Dokshicy):
                   return "Докшицы";
               case int(Areas::Vitebsk::Dubrovno):
                   return "Дубровно";
               case int(Areas::Vitebsk::Lepel):
                   return "Лепель";
               case int(Areas::Vitebsk::Liozno):
                   return "Лиозно";
               case int(Areas::Vitebsk::Miory):
                   return "Миоры";
               case int(Areas::Vitebsk::Novolukoml):
                   return "Новолукомль";
               case int(Areas::Vitebsk::Novopolock):
                   return "Новополоцк";
               case int(Areas::Vitebsk::Orsha):
                   return "Орша";
               case int(Areas::Vitebsk::Polock):
                   return "Полоцк";
               case int(Areas::Vitebsk::Postavy):
                   return "Поставы";
               case int(Areas::Vitebsk::Rossony):
                   return "Россоны";
               case int(Areas::Vitebsk::Senno):
                   return "Сенно";
               case int(Areas::Vitebsk::Tolochin):
                   return "Толочин";
               case int(Areas::Vitebsk::Ushachi):
                   return "Ушачи";
               case int(Areas::Vitebsk::Chashniki):
                   return "Чашники";
               case int(Areas::Vitebsk::Sharkovshchina):
                   return "Шарковщина";
               case int(Areas::Vitebsk::Shumilino):
                   return "Шумилино";
               case int(Areas::Vitebsk::Others):
                   return "Другое (Витебск)";
               default:
                   return "[Неизвестный регион]";
            }
        }
    }
};


