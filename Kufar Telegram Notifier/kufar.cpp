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

    optional<string> PriceRange::joinPrice() const {
        if (!priceMin.has_value() && !priceMax.has_value()) { return nullopt; }
        string joinedPrice = "";
        
        if (!priceMin.has_value()) {
            joinedPrice += '0';
        } else {
            joinedPrice += std::to_string(priceMin.value()) /*+ ','*/;
        }
        
        if (priceMax.has_value()){
            joinedPrice = "r:" + joinedPrice + ',' + std::to_string(priceMax.value());
        }
        
        return joinedPrice;
    }

    namespace {
        void insertImageURL (vector<string> &images, const string &id, const bool yams_storage) {
            if (yams_storage){
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
        
        void addURLParameter(ostringstream &ostream, const string &parameter, const optional<int> &value, const bool encodeValue = false) {
            if (value.has_value()) {
                addURLParameter(ostream, parameter, to_string(value.value()), encodeValue);
            }
        }
    }

    vector<Ad> getAds(const KufarConfiguration &configuration){
        vector<Ad> adverts;
        ostringstream urlStream;
        urlStream << baseURL;
        
        addURLParameter(urlStream, "query", configuration.tag, true);
        addURLParameter(urlStream, "lang", configuration.language);
        addURLParameter(urlStream, "ot", configuration.onlyTitleSearch);
        addURLParameter(urlStream, "size", configuration.limit);
        addURLParameter(urlStream, "rgn", configuration.region);
        addURLParameter(urlStream, "prc", configuration.priceRange.joinPrice());
        if (configuration.areas.has_value()) { addURLParameter(urlStream, "ar=v.or:", joinIntVector(configuration.areas.value(), ",")); }
        
        cout << configuration.priceRange.joinPrice() << endl;
        //addURLParameter(urlStream, "prc", )

        //TODO: Add missing parameters

        //auto x = urlStream.str();
        //cout << x << endl;
        string rawJson = getJSONFromURL(urlStream.str());
        
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
