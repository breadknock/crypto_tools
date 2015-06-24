#include <vector>
#include <string>
#include "DataStream.h"

class InfiniteStream : DataStream {
    std::vector<unsigned char> data;
public:

    const DataStream operator^(const DataStream &other) const;
};
