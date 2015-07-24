#include "DataStream.h"
#include "Sha1.h"
#include <iostream>
#include <cstdint>


uint32_t rotl(uint32_t x, int shift) {
    return (x << shift) | (x >> (32 - shift));
}

void iterate_hash(const DataStream &block, std::vector<uint32_t> &hashes) {
    std::vector<DataStream> key_blocks = block.chunk(4);
    std::vector<uint32_t> keys;
    for(int i = 0; i < 16; i++) {
        std::vector<unsigned char> block_data = key_blocks[i].get_data();
        uint32_t key = 0;
        for(int j = 0; j < 4; j++) {
            key |= block_data[j] << ((3-j)*8);
        }
        keys.push_back(key);
    }
    for(int i = 16; i < 80; i++) {
        uint32_t key = 0;
        key ^= keys[i-3];
        key ^= keys[i-8];
        key ^= keys[i-14];
        key ^= keys[i-16];
        key = rotl(key, 1);
        keys.push_back(key);
    }
        uint32_t a = hashes[0];
        uint32_t b = hashes[1];
        uint32_t c = hashes[2];
        uint32_t d = hashes[3];
        uint32_t e = hashes[4];

    for(int i = 0; i < 80; i++) {
        uint32_t f;
        uint32_t k;
        if(i < 20) {
            f = (b & c) | ((~b) & d);
            k = 0x5A827999;
        } else if(i < 40) {
            f = b ^ c ^ d;
            k = 0x6ED9EBA1;
        } else if(i < 60) {
            f = (b & c) | (b & d) | (c & d);
            k = 0x8F1BBCDC;
        } else {
            f = b ^ c ^ d;
            k = 0xCA62C1D6;
        }


        uint32_t tmp = (rotl(a, 5) + f + e + k + keys[i]);
        e = d;
        d = c;
        c = rotl(b, 30);
        b = a;
        a = tmp;
    }

    hashes[0] += a;
    hashes[1] += b;
    hashes[2] += c;
    hashes[3] += d;
    hashes[4] += e;
}


DataStream sha1_hash(const DataStream &message) {
    std::vector<unsigned char> data = message.get_data();
    uint64_t length = data.size();
    data.push_back(0x80);
    for(int i = length + 1; i % 64 != 56; i++) {
        data.push_back(0);
    }
    //big endian
    for(int j = 0; j < 8; j++) {
        data.push_back(((length*8) >> (8*(7-j))) & 0xff);
    }
    DataStream new_msg = DataStream(data);
    std::vector<DataStream> blocks = new_msg.chunk(64);

    //initialize hash
    std::vector<uint32_t> hashes;
    hashes.push_back(0x67452301);
    hashes.push_back(0xEFCDAB89);
    hashes.push_back(0x98BADCFE);
    hashes.push_back(0x10325476);
    hashes.push_back(0xC3D2E1F0);
    
    for(const DataStream &ds : blocks) {
        iterate_hash(ds,hashes);
    }
    std::vector<unsigned char> result;
    for(uint32_t h : hashes) {
        for(int i = 0; i < 4; i++) {
            result.push_back((h>>((3-i)*8))&0xff);
        }
    }
    return DataStream(result);
}

DataStream sha1_hash_with_key(DataStream key, const DataStream &message) {
    key.append(message);
    return sha1_hash(key);
}
