#include "RC4.h"
#include "DataStream.h"
#include <vector>
#include <algorithm>

std::vector<unsigned char> expand_key(const std::vector<unsigned char> &key) {
    size_t key_size = key.size();
    std::vector<unsigned char> state(256);
    for(unsigned int c = 0; c < 256; c++) {
        state[c] = c;
    }
    unsigned char j = 0;
    for(unsigned int i = 0; i < 256; i++) {
        j = (j + state[i] + key[i % key_size]) % 256;
        //swap
        std::iter_swap(state.begin() + i, state.begin() + j);
    }
    return state;
}

DataStream rc4_encrypt(const DataStream &input, const DataStream &key) {
    std::vector<unsigned char> &&state = expand_key(key.get_data());
    std::vector<unsigned char> stream(input.get_size());
    unsigned char i = 0;
    unsigned char j = 0;
    for(unsigned int k = 0; k < input.get_size(); k++) {
        i = (i + 1) % 256;
        j = (j + state[i]) % 256;
        //swap
        std::iter_swap(state.begin() + i, state.begin() + j);
        stream[k] = state[(state[i] + state[j]) % 256];
    }
    return input ^ DataStream(stream);
}
