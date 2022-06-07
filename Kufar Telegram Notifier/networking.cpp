//
//  networking.cpp
//  Kufar Telegram Notifier
//
//  Created by Macintosh on 04.06.2022.
//

#include <iostream>
#include <curl/curl.h>
#include "networking.hpp"
#include "helperfunctions.hpp"

namespace Networking {
    using std::string;

    namespace {
        static size_t writeFunction(void *ptr, size_t size, size_t nmemb, string *data) {
            data->append((char*)ptr, size * nmemb);
            return size * nmemb;
        }
    }

    string urlEncode(const string &text){
        CURL *curl = curl_easy_init();
        char *encoded = curl_easy_escape(curl, text.c_str(), 0);
        string tempVariable = encoded;
        curl_free(encoded);
        curl_easy_cleanup(curl);
        return tempVariable;
    }

    string getJSONFromURL(const string &url){
        DEBUG_MSG("[URL: " << url << "]");
        
        //curl_global_init(CURL_GLOBAL_DEFAULT);
        auto curl = curl_easy_init();
        string responseString;
        
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
            curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 0L);
            curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
            
            string headerString;
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);
            curl_easy_setopt(curl, CURLOPT_HEADERDATA, &headerString);
            
            curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            curl = NULL;
        }
        return responseString;
    }

}
