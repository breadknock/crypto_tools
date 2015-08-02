#include "RSA.h"
#include "DataStream.h"
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/miller_rabin.hpp>
#include <utility>
using boost::multiprecision::cpp_int;


RSAClient::RSAClient(size_t _bit_size, int _e) {
    bit_size = _bit_size;
    e = _e;
    cpp_int p = gen_prime(bit_size, _e);
    cpp_int q = gen_prime(bit_size, _e);
    n = p * q;
    cpp_int toit = (p - 1) * (q - 1); // since (p,q) are prime
    do {
        d = inv_mod(e, toit);
        if(!d) e++;
    } while (d == 0);
}

RSA_key RSAClient::get_public_key() {
    return std::make_pair(e, n);
}

DataStream RSAClient::decrypt(const DataStream &cipher) {
    return DataStream(powm(cipher.getCppInt(), d, n), bit_size*2);
}

DataStream encrypt(const DataStream &ds, const RSA_key &key, size_t bit_size) {
    return DataStream(powm(ds.getCppInt(), key.first, key.second), bit_size*2);
}

cpp_int inv_mod(const cpp_int &num, const cpp_int &mod) {
    cpp_int r0 = mod, r1 = num;
    cpp_int t0 = 0, t1 = 1;
    while(r1 != 0) {
        cpp_int q = r0 / r1;
        cpp_int r2 = r0 - q * r1;
        cpp_int t2 = t0 - q * t1;
        r0 = r1;
        r1 = r2;
        t0 = t1;
        t1 = t2;
    }
    if(r0 > 1) {
        return 0;
    }
    return (t0 + mod) % mod;
}


cpp_int gen_prime(size_t bit_size, int no_div) {
    cpp_int prime;
    do {
        if(no_div > 0) {
            do {
                prime = get_random_key(bit_size / 8).getCppInt();
            } while ((prime - 1) % no_div == 0);
        } else {
            prime = get_random_key(bit_size / 8).getCppInt();
        }
    } while(!miller_rabin_test(prime, 100));
    return prime;
}
