#include <vector>
#include <string>

class DataStream {
    std::vector<unsigned int> data;
    public:    
    void setAsciiString(const std::string &ascii);
    void setHexString(const std::string &hex);
    void setBase64String(const std::string &b64);
    void setBinaryString(const std::string &bin);

    std::string getAsciiString() const;
    std::string getHexString() const;
    std::string getBase64String() const;
    std::string getBinaryString() const;

    DataStream operator^(const DataStream &other) const;
    double getScore() const;
};
