//
//  helperfunctions.cpp
//  Kufar Telegram Notifier
//
//  Created by Macintosh on 04.06.2022.
//

#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iterator>

using namespace std;

bool vectorContains(const vector<int> &vector, const int &value) {
    if (find(vector.begin(), vector.end(), value) != vector.end()){
        return true;
    }
    return false;
}

string getTextFromFile(const string &path){
    ifstream ifs(path);
    return string((istreambuf_iterator<char>(ifs)),
                  (istreambuf_iterator<char>()));
}

time_t zuluToTimestamp(const string &zuluDate) {
    tm t{};
    istringstream stringStream(zuluDate);
    
    stringStream >> get_time(&t, "%Y-%m-%dT%H:%M:%S");
    if (stringStream.fail()) {
        throw runtime_error{"failed to parse time string"};
    }
    
    return mktime(&t);
}

string joinIntVector(const vector<int> &nums, const string &delim) {
    stringstream result;
    copy(nums.begin(), nums.end(), std::ostream_iterator<int>(result, delim.c_str()));
    
    string temp = result.str();
    
    if (!temp.empty()) {
        temp.pop_back();
    }
    
    return temp;
}

time_t timestampShift(const time_t &timestamp, int shift){
    return timestamp + (3600 * shift);
}
