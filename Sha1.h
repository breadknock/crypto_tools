#include "DataStream.h"

void iterate_hash(const DataStream &message, std::vector<uint32_t> &hashes);

DataStream sha1_hash(const DataStream &message);

DataStream sha1_hash_with_key(DataStream key, const DataStream &message);

DataStream sha1_hmac_hash(const DataStream &key, const DataStream &message);
