#include "DataStream.h"
#include <vector>
#include <string>
#include <ios>
#include <stringstream>


void DataStream::setAsciiString(const std::string &ascii) {
    data = std::vector<unsigned int>();
    for(const char &c : ascii) {
        data.push_back(c);
    }
}

void DataStream::setHexString(const std::string &hex) {
    data = std::vector<unsigned int>();
    for(int i = 0; i < hex.size(); i+= 2) {
        char ans[3];
        ans[2] = '\0';
        ans[0] = hex[i];
        if(i < hex.size() - 1) {
            ans[1] = hex[i+1];
        } else {
            ans[1] = '0';
        }
        data.push_back(strtol(ans,NULL,16))
    }
}
void DataStream::setBase64String(const std::string &b64) {
    data = std::vector<unsigned int>();
}

void DataStream::setBinaryString(const std::string &bin) {
    data = std::vector<unsigned int>();
}

std::string DataStream::getAsciiString() const {
    std::string answer;
    for(const unsigned char &c : data) {
        answer.push_back(c);
    }
    return answer;
}

std::string DataStream::getHexString() const {
    std::string answer;
}

std::string DataStream::getBase64String() const {
    std::string answer;
}

std::string DataStream::getBinaryString() const {
    std::string answer;
}

