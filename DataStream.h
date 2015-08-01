#include <vector>
#include <string>
#include <utility>
#include <boost/multiprecision/cpp_int.hpp>
using boost::multiprecision::cpp_int;

#ifndef __Data_Stream
#define __Data_Stream
class DataStream {
    std::vector<unsigned char> data;
    bool isInfinite = false;
    public:    
    DataStream() {}
    DataStream(std::vector<unsigned char> d) : data(d) {}
    DataStream(std::string s) { setAsciiString(s); }
    DataStream(const DataStream &ds) : data(std::vector<unsigned char>(ds.data)) {}
    DataStream(const cpp_int &cint) { setCppInt(cint); }
    void setInfinite();
    void setAsciiString(const std::string &ascii);
    void setHexString(const std::string &hex);
    void setBase64String(const std::string &b64);
    void setBinaryString(const std::string &bin);
    void setCppInt(const cpp_int &cint);

    std::string getAsciiString() const;
    std::string getHexString() const;
    std::string getBase64String() const;
    std::string getBinaryString() const;
    cpp_int getCppInt() const;

    std::vector<unsigned char> get_data() const { return data; }

    DataStream operator^(const DataStream &other) const;
    bool operator==(const DataStream &other) const;
    double getScore() const;
    int getDistance(const DataStream &other) const;
    std::pair<DataStream,DataStream> split(int index) const;
    std::vector<DataStream> chunk(unsigned int length) const;
    std::vector<DataStream> partition(int index) const;
    DataStream rotate(int index) const;
    void append(const DataStream &other);
    void push_back(unsigned char input);
    unsigned int get_size() const;
};

DataStream get_random_key(int length=16);
#endif
