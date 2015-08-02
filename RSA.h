#ifndef __RSA_h
#define __RSA_h
#include <boost/multiprecision/cpp_int.hpp>
#include "DataStream.h"
#include <utility>
using boost::multiprecision::cpp_int;

typedef std::pair<cpp_int, cpp_int> RSA_key;

class RSAClient {
private:
    size_t bit_size;
    cpp_int e;
    cpp_int d;
    cpp_int n;

public:
    RSAClient(size_t _bit_size=1024, int e=3);

    RSA_key get_public_key();

    DataStream decrypt(const DataStream &ds);

};

DataStream encrypt(const DataStream &ds, const RSA_key &key, size_t bit_size=1024);

cpp_int inv_mod(const cpp_int &num, const cpp_int &mod);

cpp_int gen_prime(size_t bit_size, int no_div=0);
#endif
