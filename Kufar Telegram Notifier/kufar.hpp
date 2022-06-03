//
//  kufar.hpp
//  Kufar Telegram Notifier
//
//  Created by Macintosh on 02.06.2022.
//

#ifndef kufar_hpp
#define kufar_hpp

#include <iostream>


namespace Kufar {
    using namespace std;
    using namespace Networking;
    using nlohmann::json;

    const string baseURL = "https://searchapi.kufar.by/v1/search/rendered-paginated?";

    enum class Region {
        Brest = 1,
        Gomel = 2,
        Grodno = 3,
        Mogilev = 4,
        Minsk_Region = 5,
        Vitebsk = 6,
        Minsk = 7
    };

    struct Areas {
        enum class Minsk {
            Centralnyj = 22,
            Sovetskij = 23,
            Pervomajskij = 24,
            Partizanskij = 25,
            Zavodskoj = 26,
            Leninskij = 27,
            Oktyabrskij = 28,
            Moskovskij = 29,
            Frunzenskij = 30
        };
        
        enum class Brest {
            Brest = 1,
            Baranovichi = 37,
            Bereza = 38,
            Beloozyorsk = 123,
            Gancevichi = 48,
            Drogichin = 49,
            Zhabinka = 50,
            Ivanovo = 51,
            Ivacevichi = 52,
            Kamenec = 53,
            Kobrin = 2,
            Luninec = 3,
            Lyahovichi = 54,
            Malorita = 55,
            Pinsk = 4,
            Pruzhany = 56,
            Stolin = 57,
            Others = 31
        };
        
        enum class Gomel {
            Gomel = 5,
            Bragin = 128,
            BudaKoshelevo = 58,
            Vetka = 59,
            Dobrush = 60,
            Elsk = 61,
            Zhitkovichi = 62,
            Zhlobin = 6,
            Kalinkovichi = 63,
            Korma = 129,
            Lelchicy = 130,
            Loev = 131,
            Mozyr = 7,
            Oktyabrskij = 132,
            Narovlya = 64,
            Petrikov = 65,
            Rechica = 8,
            Rogachev = 66,
            Svetlogorsk = 39,
            Hojniki = 67,
            Chechersk = 68,
            Others = 32
        };
        
        enum class Grodno {
            Grodno = 9,
            Berezovka = 69,
            Berestovica = 133,
            Volkovysk = 40,
            Voronovo = 134,
            Dyatlovo = 70,
            Zelva = 135,
            Ive = 71,
            Korelichi = 136,
            Lida = 10,
            Mosty = 72,
            Novogrudok = 73,
            Ostrovec = 74,
            Oshmyany = 75,
            Svisloch = 76,
            Skidel = 77,
            Slonim = 11,
            Smorgon = 41,
            Shchuchin = 78,
            Others = 33
        };
        
        enum class Mogilev {
            Mogilev = 13,
            Belynichi = 137,
            Bobrujsk = 12,
            Byhov = 79,
            Glusk = 80,
            Gorki = 42,
            Dribin = 138,
            Kirovsk = 81,
            Klimovichi = 82,
            Klichev = 83,
            Krasnopole = 139,
            Krugloe = 140,
            Kostyukovichi = 84,
            Krichev = 43,
            Mstislavl = 85,
            Osipovichi = 14,
            Slavgorod = 86,
            Chausy = 87,
            Cherikov = 88,
            Shklov = 89,
            Hotimsk = 141,
            Others = 34
        };
        
        enum class MinskRegion {
            MinskRegion = 142,
            Berezino = 91,
            Borisov = 15,
            Vilejka = 92,
            Volozhin = 93,
            Dzerzhinsk = 94,
            Zhodino = 44,
            Zaslavl = 143,
            Kleck = 95,
            Kopyl = 96,
            Krupki = 97,
            Logojsk = 98,
            Lyuban = 99,
            MarinaGorka = 122,
            Molodechno = 16,
            Myadel = 100,
            Nesvizh = 101,
            Rudensk = 145,
            Sluck = 17,
            Smolevichi = 102,
            Soligorsk = 45,
            StaryeDorogi = 103,
            Stolbcy = 104,
            Uzda = 105,
            Fanipol = 144,
            Cherven = 106,
            Others = 35
        };
        
        enum class Vitebsk {
            Vitebsk = 18,
            Beshenkovichi = 125,
            Baran = 107,
            Braslav = 108,
            Verhnedvinsk = 109,
            Glubokoe = 110,
            Gorodok = 111,
            Dokshicy = 112,
            Dubrovno = 113,
            Lepel = 114,
            Liozno = 115,
            Miory = 116,
            Novolukoml = 117,
            Novopolock = 46,
            Orsha = 19,
            Polock = 20,
            Postavy = 47,
            Rossony = 118,
            Senno = 119,
            Tolochin = 120,
            Ushachi = 126,
            Chashniki = 121,
            Sharkovshchina = 127,
            Shumilino = 124,
            Others = 36
        };
    };

    namespace {
        static const string PROPERTY_UNDEFINED = "[UNDEFINED]";
        
        template<typename T>
        optional<T> get_at_optional(const json &obj, const string &key) try {
            return obj.at(key).get<T>();
        } catch (...) {
            return nullopt;
        }
        
        template<typename T>
        ostream &operator << (ostream &os, optional<T> const &opt){
            return opt ? (os << opt.value()) : (os << PROPERTY_UNDEFINED);
        }
    
        time_t zuluToTimestamp(const string &zuluDate) {
            tm t{};
            istringstream stringStream(zuluDate);
            
            stringStream >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S");
            if (stringStream.fail()) {
                throw std::runtime_error{"failed to parse time string"};
            }
            
            return mktime(&t);
        }
    
        string joinIntVector(const vector<int> &nums, const string &delim){
            stringstream result;
            copy(nums.begin(), nums.end(), std::ostream_iterator<int>(result, delim.c_str()));
            return result.str();
        }
    };

    namespace {
        void insertImageURL (vector<string> &images, const string &id, const bool yams_storage){
            if (yams_storage){
                images.push_back("https://yams.kufar.by/api/v1/kufar-ads/images/" + id.substr(0, 2) + "/" + id +    ".jpg?rule=pictures");
            }
        }
    }

    namespace {
        struct Ad {
            string tag;
            string title;
            int id;
            time_t date;
            int price;
            string sellerName;
            bool phoneNumberIsVisible;
            string link;
            vector<string> images;
        };
    };

    struct KufarConfiguration {
        string tag;
        bool onlyTitleSearch = true;
        optional<int> priceMin;
        optional<int> priceMax;
        
        string language;
        int limit;
        Kufar::Region region;
        vector<int> areas;
    };
    
    vector<Ad> getAds(const KufarConfiguration &configuration){
        vector<Ad> adverts;
        string rawJson = getJSONFromURL(
                                        baseURL +
                                        "lang=" + configuration.language + "&"
                                        "query=" + urlEncode(configuration.tag) + "&"
                                        "ot=" + to_string(configuration.onlyTitleSearch) + "&"
                                        "size=" + to_string(configuration.limit) + "&"
                                        "ar=v.or:" + joinIntVector(configuration.areas, ",") + "&rgn=" + to_string((int)configuration.region)
        );
        
        json ads = json::parse(rawJson).at("ads");

        for (const auto &ad : ads){
            Ad advert;
            
            advert.tag = configuration.tag;
            advert.title = ad.at("subject");
            advert.id = ad.at("ad_id");
            advert.date = zuluToTimestamp((string)ad.at("list_time"));
            advert.price = stoi((string)ad.at("price_byn"));
            advert.phoneNumberIsVisible = !ad.at("phone_hidden");
            
            json accountParameters = ad.at("account_parameters");
            for (const auto &accountParameter : accountParameters){
                if (accountParameter.at("p") == "name"){
                    advert.sellerName = accountParameter.at("v");
                    break;
                }
            }
            
            json imagesArray = ad.at("images");
            for (const auto &image : imagesArray){
                string imageID = image.at("id");
                bool isYams = image.at("yams_storage");
                insertImageURL(advert.images, imageID, isYams);
            }
            
            adverts.push_back(advert);
        }
        
        
        return adverts;
    }
    
};

#endif /* kufar_hpp */
