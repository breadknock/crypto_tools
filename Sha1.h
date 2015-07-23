#include "DataStream.h"

DataStream sha1_hash(const DataStream &message);

DataStream sha1_hash_with_key(DataStream key, const DataStream &message);
