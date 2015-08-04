#include "DSA.h"
#include "DataStream.h"
#include "Sha1.h"
#include <utility>
#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>

cpp_int mod_inv(cpp_int k, cpp_int mod) {
    return powm(k, mod - 2, mod);
}

DSAClient DSAMaster::generate_client(cpp_int x) {
    while(x == 0) {
        x = get_random_key(msb(q) / 8 + 2).getCppInt() % q;
        std::cout<<x<<std::endl;
    }
    return DSAClient(p,q,g,x);
}

DSAClient::DSAClient(const cpp_int &_p, const cpp_int &_q, const cpp_int &_g, const cpp_int &_x) : p(_p), q(_q), g(_g), x(_x) {
    y = powm(g, x, p);
}

DSA_Sig DSAClient::sign(const DataStream &msg, cpp_int k) {
    cpp_int r = 0;
    cpp_int s = 0;
    while(r == 0 || s == 0) { 
        if(k == 0) {
            unsigned int byte_size = msb(q) / 8 + 2;
            k = get_random_key(byte_size).getCppInt() % q;
        }
        r = (cpp_int)powm(g, k, p) % q;
        s = x * r;
        s += sha1_hash(msg).getCppInt();
        s *= mod_inv(k, q);
        s %= q;
    }
    return std::make_pair(r, s);
}

DSA_key DSAClient::get_public_key() {
    DSA_key key;
    key.p = p;
    key.q = q;
    key.g = g;
    key.y = y;
    return key;
}

bool verify(const DSA_key &pub_key, const DSA_Sig &sig, const DataStream &msg) {
    if(sig.first >= pub_key.q || sig.second >= pub_key.q) {
        return false;
    }
    cpp_int w = mod_inv(sig.second, pub_key.q);
    cpp_int u1 = sha1_hash(msg).getCppInt() * (w % pub_key.q);
    cpp_int u2 = (sig.first * w) % pub_key.q;
    cpp_int v = ((cpp_int)powm(pub_key.g, u1, pub_key.p) * (cpp_int)powm(pub_key.y, u2, pub_key.p)) % pub_key.p % pub_key.q;
    return v == sig.first;
}

DSAMaster create_default_master() {
    return DSAMaster(cpp_int("0x800000000000000089e1855218a0e7dac38136ffafa72eda7859f2171e25e65eac698c1702578b07dc2a1076da241c76c62d374d8389ea5aeffd3226a0530cc565f3bf6b50929139ebeac04f48c3c84afb796d61e5a4f9a8fda812ab59494232c7d2b4deb50aa18ee9e132bfa85ac4374d7f9091abc3d015efc871a584471bb1"),
            cpp_int("0xf4f47f05794b256174bba6e9b396a7707e563c5b"),
            cpp_int("0x5958c9d3898b224b12672c0b98e06c60df923cb8bc999d119458fef538b8fa4046c8db53039db620c094c9fa077ef389b5322a559946a71903f990f1f7e0e025e2d7f7cf494aff1a0470f5b64c36b625a097f1651fe775323556fe00b3608c887892878480e99041be601a62166ca6894bdd41a7054ec89f756ba9fc95302291"));
}
