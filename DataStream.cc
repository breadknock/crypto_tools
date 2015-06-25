#include "DataStream.h"
#include <vector>
#include <string>
#include <ios>
#include <sstream>

void DataStream::setInfinite() {
    isInfinite = true;
}

void DataStream::setAsciiString(const std::string &ascii) {
    data = std::vector<unsigned char>();
    for(const char &c : ascii) {
        data.push_back(c);
    }
}

void DataStream::setHexString(const std::string &hex) {
    data = std::vector<unsigned char>();
    for(int i = 0; i < hex.size(); i+= 2) {
        char ans[3];
        ans[2] = '\0';
        ans[0] = hex[i];
        if(i < hex.size() - 1) {
            ans[1] = hex[i+1];
        } else {
            ans[1] = '0';
        }
        data.push_back(strtol(ans,NULL,16));
    }
}
void DataStream::setBase64String(const std::string &b64) {
    data = std::vector<unsigned char>();
    for(int i = 0; i < b64.size(); i+=4) {
        unsigned long part = 0;
        int j;
        for(j = 0; j < 4; j++) {
            char c = b64[i+j];
            int tmp;
            if(c >= 'A' && c <= 'Z') {
                tmp = c - 'A';
            } else if(c >= 'a' && c <= 'z') {
                tmp = c - 'a' + 26;
            } else if(c >= '0' && c <= '9') {
                tmp = c - '0' + 52;
            } else if(c == '+') {
                tmp = 62;
            } else if(c == '/') {
                tmp = 63;
            } else if(c == '=') {
                break;
            }
            part |= tmp<<((3-j)*6);
        }
        for(int k = 0; k < j-1; k++) {
            data.push_back(part>>(8*(2-k))&0xff);
        }
    }
}

void DataStream::setBinaryString(const std::string &bin) {
    data = std::vector<unsigned char>();
}

std::string DataStream::getAsciiString() const {
    std::string answer;
    for(const unsigned char &c : data) {
        answer.push_back(c);
    }
    return answer;
}

std::string DataStream::getHexString() const {
    std::stringstream stream;
    for(const unsigned char &c : data) {
        stream << std::hex << (int) c;
    }
    return stream.str();
}

std::string DataStream::getBase64String() const {
    std::string answer;
    for(int i = 0; i < data.size(); i+=3) {
        int j;
        unsigned long part = 0;
        for(j = 0; j < 3; j++) {
            if(i+j < data.size()) {
                part |= data[i+j]<<(8*(2-j));
            } else {
                break;
            }
        }
        for(int k = 0; k < j + 1; k++) {
            unsigned char tmp = part>>(6*(3-k))&63;
            if(tmp < 26) {
                answer.push_back('A'+tmp);
            } else if(tmp < 52) {
                answer.push_back('a'+(tmp-26));
            } else if(tmp < 62) {
                answer.push_back('0'+(tmp-52));
            } else if(tmp == 62) {
                answer.push_back('+');
            } else if(tmp == 62) {
                answer.push_back('/');
            }
        }
        for(; j < 3; j++) {
            answer.push_back('=');
        }
    }
    return answer;
}

std::string DataStream::getBinaryString() const {
    std::string answer;

    return answer;
}

DataStream DataStream::operator^(const DataStream &other) const {
    int min = 0;
    if(!isInfinite) {
        min = data.size();
    }
    if(!other.isInfinite) {
        min = std::min(min,other.data.size());
    }
    std::vector<unsigned char> result;
    for(int i = 0; i < min; i++) {
        result.push_back(data[i%data.size()]^other.data[i%other.data.size()]);
    }
    DataStream ds;
    ds.data = result;
    return ds;
}

double getScoreChar(const unsigned char c) {
    if(c <= 'z' && c >= 'a') {
        return 2.1;
    } else if(c <= 'Z' && c >= 'A') {
        return 1.8;
    } else if(c == ' ') {
        return 1.0;
    } else if(c < 0x7f && c > 0x20 || c == '\n' || c == '\r') {
        return 0.1;
    } else {
        return -5.0;
    }
}

double DataStream::getScore() const {
    if(data.size() == 0) {
        return 0;
    }
    double total = 0;

    for(unsigned char c: data) {
        total += getScoreChar(c);
    }
    return total / data.size();
}
