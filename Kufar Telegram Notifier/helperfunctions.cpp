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
#include <algorithm>
#include <optional>
#include <unistd.h>
#include <limits.h>
#include <iostream>

using namespace std;

bool vectorContains(const vector<int> &vector, const int &value) {
    if (find(vector.begin(), vector.end(), value) != vector.end()) {
        return true;
    }
    return false;
}

bool fileExists(const string &path) {
    ifstream f(path);
    return f.good();
}

uint64_t getFileSize(const string &path) {
    ifstream f(path, ios::binary | ios::ate);
    return f.tellg();
}

string getTextFromFile(const string &path) {
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

time_t timestampShift(const time_t &timestamp, int shift) {
    return timestamp + (3600 * shift);
}

bool stringHasPrefix(const string &originalString, const string &prefix) {
    return originalString.rfind(prefix, 0) == 0;
}

#ifdef __APPLE__
    #include <mach-o/dyld.h>
    #include <filesystem>

    optional<string> getWorkingDirectory() {
        
        /*char temp[PATH_MAX];
        return (getcwd(temp, sizeof(temp)) ? std::string( temp ) : std::string("") );*/
        char buffer[PATH_MAX];
        uint32_t buffsize = PATH_MAX;
        
        
        if (_NSGetExecutablePath(buffer, &buffsize) == 0) {
            return ((filesystem::path)buffer).parent_path();
        }
        
        return nullopt;
    }
#elif __linux__
    #include <libgen.h>
    #include <linux/limits.h>

    optional<string> getWorkingDirectory() {
        char result[PATH_MAX];
        size_t count = readlink("/proc/self/exe", result, PATH_MAX);
        if (count != -1) {
            return dirname(result);
        }
        return nullopt;
    }
#else
    optional<string> getWorkingDirectory() { return nullopt; }
#endif
