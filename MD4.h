#include "DataStream.h"

void iterate_md4_hash(const DataStream &message, std::vector<uint32_t> &hashes);

DataStream md4_hash(const DataStream &message);

DataStream md4_hash_with_key(DataStream key, const DataStream &message);
