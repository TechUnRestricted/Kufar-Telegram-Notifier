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
        
    time_t zuluToTimestamp(const string &zuluDate) {
        tm t{};
        istringstream stringStream(zuluDate);
        
        stringStream >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S");
        if (stringStream.fail()) {
            throw std::runtime_error{"failed to parse time string"};
        }
        
        return mktime(&t);
    }

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
    };

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
    
    namespace {
        void insertImageURL (vector<string> &images, const string &id, const bool yams_storage){
            if (yams_storage){
                images.push_back("https://yams.kufar.by/api/v1/kufar-ads/images/" + id.substr(0, 2) + "/" + id + ".jpg?rule=pictures");
            }
        }
    }

    struct Query {
        string tag;
        bool onlyTitleSearch = true;
        optional<int> priceMin;
        optional<int> priceMax;
    };

    struct Configuration {
        vector<Query> queries;
        string language;
        int limit;
    };
    
    vector<Ad> getAds(const Configuration &configuration){
        vector<Ad> adverts;
        for (int i = 0; i < configuration.queries.size(); i++){            
            string rawJson = getJSONFromURL(
                                            baseURL +
                                            "lang=" + configuration.language + "&"
                                            "query=" + urlEncode(configuration.queries[i].tag) + "&"
                                            "ot=" + to_string(configuration.queries[i].onlyTitleSearch) + "&"
                                            "size=" + to_string(configuration.limit) + "&"
                                            "ar=v.or:9,74&rgn=3"
            );
            
            json ads = json::parse(rawJson).at("ads");
            cout << ads.size() << endl;
            for (int i = 0; i < ads.size(); i++){
                Ad advert;
                
                advert.tag = configuration.queries[i].tag;
                advert.title = ads[i].at("subject");
                advert.id = ads[i].at("ad_id");
                advert.date = zuluToTimestamp((string)ads[i].at("list_time"));
                advert.price = stoi((string)ads[i].at("price_byn"));
                advert.phoneNumberIsVisible = !ads[i].at("phone_hidden");
                
                json accountParameters = ads[i].at("account_parameters");
                for (int i = 0; i < accountParameters.size(); i++){
                    if (accountParameters[i].at("p") == "name"){
                        advert.sellerName = accountParameters[i].at("v");
                        break;
                    }
                }
                
                json imagesArray = ads[i].at("images");
                for (int i = 0; i < imagesArray.size(); i++){
                    string imageID = imagesArray[i].at("id");
                    bool isYams = imagesArray[i].at("yams_storage");
                    insertImageURL(advert.images, imageID, isYams);
                }
                adverts.push_back(advert);
            }
        }
        
        return adverts;
    }

};

#endif /* kufar_hpp */
