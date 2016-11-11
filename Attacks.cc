#include "Attacks.h"
#include "RSA.h" //inv mod function

std::pair<cpp_int, cpp_int> find_common_modulus(const std::vector<std::pair<cpp_int, cpp_int>> &congruences) {
    cpp_int x = 0;
    cpp_int product = 1;
    for(auto pr : congruences) {
        product *= pr.second;
    }
    for(size_t i = 0; i < congruences.size(); i++) {
        cpp_int b = inv_mod(product / congruences[i].second, congruences[i].second);
        x += congruences[i].first*b*(product/congruences[i].second);
    }
    x %= product;
    return std::make_pair(x, product);
}
