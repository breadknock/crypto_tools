#include "Twister.h"
#include <vector>
#include <cstdint>
#include <iostream>


Twister::Twister(uint32_t seed) {
    state.push_back(seed);
    for(int i = 1; i < VECTOR_SIZE; i++) {
        uint32_t last = state.back();
        state.push_back(INIT_FACTOR * (last ^ (last >> (NUM_SIZE - 2))) + i);
    }
    index = VECTOR_SIZE;
}

void Twister::generate_next_block() {
    for(int i = 0; i < VECTOR_SIZE; i++) {
        uint32_t x = state[i] & UPPER_MASK;
        x += state[(i + 1) % VECTOR_SIZE] & LOW_MASK;
        uint32_t shifted = x >> 1;
        if(x & 1) {
            shifted ^= FIELD_OVERFLOW;
        }
        state[i] = state[(i + MIDDLE_OFFSET) % VECTOR_SIZE] ^ shifted;
    }
    index = 0;
}


uint32_t Twister::get_random() {
    if(index >= VECTOR_SIZE) {
        generate_next_block();
    }
    uint32_t raw = state[index];
    raw ^= ((raw >> SHIFT_1) & MASK_1);
    raw ^= ((raw << SHIFT_2) & MASK_2);
    raw ^= ((raw << SHIFT_3) & MASK_3);
    raw ^= (raw >> SHIFT_4);
    index++;
    return raw;
}
