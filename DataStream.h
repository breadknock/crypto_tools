#include <vector>
#include <string>
#include <utility>

#ifndef __Data_Stream
#define __Data_Stream
class DataStream {
    std::vector<unsigned char> data;
    bool isInfinite = false;
    public:    
    DataStream() {}
    DataStream(std::vector<unsigned char> d) { data = d; }
    void setInfinite();
    void setAsciiString(const std::string &ascii);
    void setHexString(const std::string &hex);
    void setBase64String(const std::string &b64);
    void setBinaryString(const std::string &bin);

    std::string getAsciiString() const;
    std::string getHexString() const;
    std::string getBase64String() const;
    std::string getBinaryString() const;

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
};

#endif
