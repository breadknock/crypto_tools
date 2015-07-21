#include <vector>
#include <cstdint>

#ifndef __twister_h
#define __twister_h



class Twister {
private:
    //CONSTANTS
    const int VECTOR_SIZE = 624;
    const int NUM_SIZE = 32;
    const int MIDDLE_OFFSET = 397;
    const uint32_t INIT_FACTOR = 0x6C078965;
    const uint32_t FIELD_OVERFLOW = 0x9908B0DF;
    const uint32_t LOW_MASK = (1U << 31)-1;
    const uint32_t UPPER_MASK = ~LOW_MASK;


    const int SHIFT_1 = 11;
    const uint32_t MASK_1 = 0xFFFFFFFF;

    const int SHIFT_2 = 7;
    const uint32_t MASK_2 = 0x9D2C5680;

    const int SHIFT_3 = 15;
    const uint32_t MASK_3 = 0xEFC60000;

    const int SHIFT_4 = 18;

    //INTERNAL STATE
    std::vector<uint32_t> state;
    int index;

    void generate_next_block();
public:
    Twister(uint32_t seed);
    uint32_t get_random();
    void set_state(std::vector<uint32_t> new_state) { state = new_state; index = VECTOR_SIZE; }
};

#endif
