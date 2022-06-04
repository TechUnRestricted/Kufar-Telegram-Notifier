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

    namespace {
        void insertImageURL (vector<string> &images, const string &id, const bool yams_storage) {
            if (yams_storage){
                images.push_back("https://yams.kufar.by/api/v1/kufar-ads/images/" + id.substr(0, 2) + "/" + id + ".jpg?rule=pictures");
            }
        }
    }
    
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
            advert.date = timestampShift(zuluToTimestamp((string)ad.at("list_time")), 3);
            advert.price = stoi((string)ad.at("price_byn"));
            advert.phoneNumberIsVisible = !ad.at("phone_hidden");
            advert.link = ad.at("ad_link");
            
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
