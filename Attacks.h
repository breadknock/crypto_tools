#include <boost/multiprecision/cpp_int.hpp>
#include <vector>
#include <utility>
using boost::multiprecision::cpp_int;

std::pair<cpp_int, cpp_int> find_common_modulus(const std::vector<std::pair<cpp_int, cpp_int>> &congruences);
