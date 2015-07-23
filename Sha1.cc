#include "DataStream.h"
#include "Sha1.h"

DataStream sha1_hash(const DataStream &message) {
    //IMPLEMENT
    return message;
}

DataStream sha1_hash_with_key(const DataStream &message, DataStream key) {
    key.append(message);
    return sha1_hash(key);
}
