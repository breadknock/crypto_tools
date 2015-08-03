#include "DSA.h"
#include "DataStream.h"
#include "Sha1.h"
#include <utility>
#include <boost/multiprecision/cpp_int.hpp>

DSAClient DSAMaster::generate_client(const cpp_int &x) {
    if(x == 0) {
        //generate random number
    }
    return DSAClient(x);
}

DSAClient::DSAClient(const cpp_int &_p, const cpp_int &_q, const cpp_int &_g, const cpp_int &_x) : p(_p), q(_q), g(_g), x(_x) {
    y = powm(g, x, p);
}

DSA_Sig DSAClient::sign(const DataStream &msg, const cpp_int &k) {
    if(k == 0) {
        k = get_random_key(bit_size / 8)
        //generate k;
    }
    //sign message

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
    if(sig.first >= q || sig.second >= q) {
        return false;
    }
    //Finish verification

}

DSAMaster create_default_master() {
    return DSAMaster(cpp_int("0x800000000000000089e1855218a0e7dac38136ffafa72eda7859f2171e25e65eac698c1702578b07dc2a1076da241c76c62d374d8389ea5aeffd3226a0530cc565f3bf6b50929139ebeac04f48c3c84afb796d61e5a4f9a8fda812ab59494232c7d2b4deb50aa18ee9e132bfa85ac4374d7f9091abc3d015efc871a584471bb1"),
            cpp_int("0xf4f47f05794b256174bba6e9b396a7707e563c5b"),
            cpp_int("0x5958c9d3898b224b12672c0b98e06c60df923cb8bc999d119458fef538b8fa4046c8db53039db620c094c9fa077ef389b5322a559946a71903f990f1f7e0e025e2d7f7cf494aff1a0470f5b64c36b625a097f1651fe775323556fe00b3608c887892878480e99041be601a62166ca6894bdd41a7054ec89f756ba9fc95302291"));
}
