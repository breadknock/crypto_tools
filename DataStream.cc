#include "DataStream.h"
#include <vector>
#include <string>
#include <ios>
#include <sstream>
#include <iomanip>
#include <utility>
#include <fstream>
#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>
using boost::multiprecision::cpp_int;

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
    for(unsigned int i = 0; i < hex.size(); i+= 2) {
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
    for(unsigned int i = 0; i < b64.size(); i+=4) {
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
            } else {
                tmp = 64;
                throw 1;
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

void DataStream::setCppInt(const cpp_int &cint) {
    std::stringstream ss;
    ss << std::hex << cint;
    std::string hex_int = ss.str();
    std::string prefix;
    if(hex_int.size()%2) {
        prefix = "0";
    }
    prefix += hex_int;
    setHexString(prefix);
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
        stream << std::setfill('0') << std::setw(2) << std::hex << (int) c;
    }
    return stream.str();
}

std::string DataStream::getBase64String() const {
    std::string answer;
    for(unsigned int i = 0; i < data.size(); i+=3) {
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
            } else if(tmp == 63) {
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
    for(unsigned char c : data) {
        for(int i = 0; i < 8; i++) {
            answer.push_back(((c << (7-i))&1)?'1':'0');
        }
    }
    return answer;
}

cpp_int DataStream::getCppInt() const {
    std::string num = "0x";
    num += getHexString();
    return cpp_int(num);
}

DataStream DataStream::operator^(const DataStream &other) const {
    unsigned int min = -1;
    if(!isInfinite) {
        min = data.size();
    }
    if(!other.isInfinite) {
        min = std::min(min,(unsigned int)other.data.size());
    }
    std::vector<unsigned char> result;
    for(unsigned int i = 0; i < min; i++) {
        result.push_back(data[i%data.size()]^other.data[i%other.data.size()]);
    }
    DataStream ds;
    ds.data = result;
    return ds;
}

bool DataStream::operator==(const DataStream &other) const {
    return data == other.data;
}

std::pair<DataStream,DataStream> DataStream::split(int index) const {
    DataStream first;
    first.data = std::vector<unsigned char>(data.begin(),data.begin()+index);
    DataStream last;
    last.data = std::vector<unsigned char>(data.begin()+index,data.end());
    return std::make_pair(first,last);
}

std::vector<DataStream> DataStream::chunk(unsigned int size) const {
    std::vector<DataStream> chunks;
    unsigned int i = 0;
    while(i < data.size()) {
        std::vector<unsigned char> chunk;
        for(unsigned int j = 0; j < size && (i + j) < data.size(); j++) {
            chunk.push_back(data[i + j]);
        }
        i += size;
        chunks.push_back(chunk);
    }
    return chunks;
}


std::vector<DataStream> DataStream::partition(int index) const {
    std::vector<std::vector<unsigned char> > part_data;
    for(int i = 0; i < index; i++) {
        part_data.push_back(std::vector<unsigned char>());
    }
    for(unsigned int i = 0; i < data.size(); i++) {
        part_data[i%index].push_back(data[i]);
    }
    std::vector<DataStream> partitions;
    for(auto &v : part_data) {
        DataStream d;
        d.data = v;
        partitions.push_back(d);
    }
    return partitions;
}

int DataStream::getDistance(const DataStream &other) const {
    int min = std::min(data.size(),other.data.size());
    int distance = 0;
    for(int i = 0; i < min; i++) {
        for(int j = 0; j < 8; j++) {
            distance += (((data[i]>>j)&1) == ((other.data[i]>>j)&1))?0:1;
        }
    }
    return distance;
}
    


double getScoreChar(const unsigned char c) {
    if(c <= 'z' && c >= 'a') {
        return 3.1;
    } else if(c <= 'Z' && c >= 'A') {
        return 2.8;
    } else if(c == ' ') {
        return 1.0;
    } else if((c < 0x7f && c > 0x20) || c == '\n' || c == '\r') {
        return 0.1;
    } else {
        return -50.0;
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

void DataStream::append(const DataStream &other) {
    data.insert(data.end(),other.data.begin(),other.data.end());
}

DataStream DataStream::rotate(int shift) const {
    shift %= data.size();
    std::vector<unsigned char> new_data;
    for(unsigned int i = shift; i < data.size(); i++) {
        new_data.push_back(data[i]);
    }
    for(int i = 0; i < shift; i++) {
        new_data.push_back(data[i]);
    }
    return DataStream(new_data);
}

unsigned int DataStream::get_size() const {
    return data.size();
}

DataStream get_random_key(int length) {
    std::ifstream rand_file("/dev/urandom",std::ios::binary);
    unsigned char* data = new unsigned char[length];
    rand_file.read((char*)data, length);
    DataStream ds(std::vector<unsigned char>(data,data + length));
    delete[] data;
    rand_file.close();
    return ds;
}
