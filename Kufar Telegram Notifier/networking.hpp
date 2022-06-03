//
//  networking.h
//  FromHateToLove
//
//  Created by Macintosh on 02.06.2022.
//

#ifndef networking_hpp
#define networking_hpp

#include <iostream>
#include <curl/curl.h>

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
        //curl_global_init(CURL_GLOBAL_DEFAULT);
        std::cout << "[URL: " << url << "]" << std::endl;
        
        auto curl = curl_easy_init();
        string response_string;
        
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
            curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
            curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
            
            string header_string;
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
            curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);
            
            curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            curl = NULL;
        }
        return response_string;
    }
}

#endif /* networking_hpp */
