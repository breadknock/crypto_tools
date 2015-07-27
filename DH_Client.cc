#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/random.hpp>
#include <boost/multiprecision/integer.hpp>
#include "DH_Client.h"
#include "DataStream.h"
#include <iostream>
#include <time.h>
using namespace boost::multiprecision;
using namespace boost::random;

cpp_int get_random_private_key(cpp_int p) {
    boost::mt19937 rng(time(NULL));
    boost::uniform_int<cpp_int> gen(0,pow(cpp_int(2), 1600));
    cpp_int rand = gen(rng);
    return powm(rand,cpp_int(1),p);
}

DH_Client::DH_Client() {
    base = 2;
    p = cpp_int("0xffffffffffffffffc90fdaa22168c234c4c6628b80dc1cd129024e088a67cc74020bbea63b139b22514a08798e3404ddef9519b3cd3a431b302b0a6df25f14374fe1356d6d51c245e485b576625e7ec6f44c42e9a637ed6b0bff5cb6f406b7edee386bfb5a899fa5ae9f24117c4b1fe649286651ece45b3dc2007cb8a163bf0598da48361c55d39a69163fa8fd24cf5f83655d23dca3ad961c62f356208552bb9ed529077096966d670c354e4abc9804f1746c08ca237327ffffffffffffffff");
    private_key = get_random_private_key(p);
}

DH_Client::DH_Client(const cpp_int &_p, const cpp_int &_base) {
    base = _base;
    p = _p;
    private_key = get_random_private_key(p);
}

cpp_int DH_Client::get_public_key() {
    return powm(base, private_key, p);
}

cpp_int DH_Client::get_shared_secret(const cpp_int &other) {
    return powm(other, private_key, p);
}


DataStream DH_Client::get_shared_secret_data(const cpp_int &other) {
    cpp_int data = powm(other, private_key, p);
    std::vector<unsigned char> ans;
    while(data > 0) {
        cpp_int and_result = data & 255;
        ans.push_back(static_cast<unsigned char>(and_result));
        data >>= 8;
    }
    return DataStream(ans);
}
