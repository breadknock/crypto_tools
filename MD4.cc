#include "DataStream.h"
#include "MD4.h"
#include <iostream>
#include <cstdint>


uint32_t rotl2(uint32_t x, int shift) {
    return (x << shift) | (x >> (32 - shift));
}

void iterate_md4_hash(const DataStream &block, std::vector<uint32_t> &hashes) {
    std::vector<DataStream> key_blocks = block.chunk(4);
    std::vector<uint32_t> keys;
    for(int i = 0; i < 16; i++) {
        std::vector<unsigned char> block_data = key_blocks[i].get_data();
        uint32_t key = 0;
        for(int j = 0; j < 4; j++) {
            key |= block_data[j] << (j*8);
        }
        keys.push_back(key);
    }
        uint32_t a = hashes[0];
        uint32_t b = hashes[1];
        uint32_t c = hashes[2];
        uint32_t d = hashes[3];

    //Round 1
    for(int i = 0; i < 16; i+=4) {
        a = rotl2(a + ((b&c)|(~b&d)) + keys[i+0], 3);
        d = rotl2(d + ((a&b)|(~a&c)) + keys[i+1], 7);
        c = rotl2(c + ((d&a)|(~d&b)) + keys[i+2], 11);
        b = rotl2(b + ((c&d)|(~c&a)) + keys[i+3], 19);
    }
    //Round 2
    for(int i = 0; i < 4; i++) {
        a = rotl2(a + ((b&c)|(b&d)|(c&d)) + keys[i+0]  + 0x5A827999, 3);
        d = rotl2(d + ((a&b)|(a&c)|(b&c)) + keys[i+4]  + 0x5A827999, 5);
        c = rotl2(c + ((d&a)|(d&b)|(a&b)) + keys[i+8]  + 0x5A827999, 9);
        b = rotl2(b + ((c&d)|(c&a)|(d&a)) + keys[i+12]  + 0x5A827999, 13);
    }
    //Round 3
    for(int i = 0; i < 4; i++) {
        int i2;
        if(i == 1 || i == 2) {
            i2 = 3 - i;
        } else {
            i2 = i;
        }

        a = rotl2(a + (b^c^d) + keys[i2+0]  + 0x6ED9EBA1, 3);
        d = rotl2(d + (a^b^c) + keys[i2+8]  + 0x6ED9EBA1, 9);
        c = rotl2(c + (d^a^b) + keys[i2+4]  + 0x6ED9EBA1, 11);
        b = rotl2(b + (c^d^a) + keys[i2+12] + 0x6ED9EBA1, 15);
    }
    hashes[0] += a;
    hashes[1] += b;
    hashes[2] += c;
    hashes[3] += d;
}


DataStream md4_hash(const DataStream &message) {
    std::vector<unsigned char> data = message.get_data();
    uint64_t length = data.size();
    data.push_back(0x80);
    for(int i = length + 1; i % 64 != 56; i++) {
        data.push_back(0);
    }
    //big endian
    for(int j = 0; j < 8; j++) {
        data.push_back(((length*8) >> (8*(j))) & 0xff);
    }
    DataStream new_msg = DataStream(data);
    std::vector<DataStream> blocks = new_msg.chunk(64);

    //initialize hash
    std::vector<uint32_t> hashes;
    hashes.push_back(0x67452301);
    hashes.push_back(0xEFCDAB89);
    hashes.push_back(0x98BADCFE);
    hashes.push_back(0x10325476);
    
    for(const DataStream &ds : blocks) {
        iterate_md4_hash(ds,hashes);
    }
    std::vector<unsigned char> result;
    for(uint32_t h : hashes) {
        for(int i = 0; i < 4; i++) {
            result.push_back((h>>(i*8))&0xff);
        }
    }
    return DataStream(result);
}

DataStream md4_hash_with_key(DataStream key, const DataStream &message) {
    key.append(message);
    return md4_hash(key);
}
