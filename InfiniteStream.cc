#include "DataStream.h"
#include "InfiniteStream.h"
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>


const DataStream DataStream::operator^(const DataStream &other) const {
    std::vector<unsigned char> result;
    for(int i = 0; i < other.data.size(); i++) {
        result.push_back(other.data[i] ^ data[i%data.size()]);
    }
    DataStream d;
    d.data = result;
    return d;
}

