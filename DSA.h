#include "DataStream.h"
#include <boost/multiprecision/cpp_int.hpp>
#include <utility>
using boost::multiprecision::cpp_int;
typedef DSA_Sig std::pair<cpp_int, cpp_int>

struct DSA_key {
    cpp_int p;
    cpp_int q;
    cpp_int g;
    cpp_int y;
};

class DSAMaster {
private:
    cpp_int p;
    cpp_int q;
    cpp_int g;


    DSAMaster(); //not implemented
    DSAMaster(const cpp_int &p, const cpp_int &q, const cpp_int &g) : p(_p), q(_q), g(_g) {}
    DSAClient generate_client(const cpp_int &x = 0);
};

class DSAClient {
    friend class DSAMaster;
private:
    cpp_int x;
    cpp_int p;
    cpp_int q;
    cpp_int g;
    cpp_int y;
    DSAClient(const cpp_int &p, const cpp_int &q, const cpp_int &g, const cpp_int &x);
public:
    DSA_Sig sign(DataStream msg, cpp_int key k = 0);
    DSA_key get_public_key()

};
bool verify(DSA_key pub_key, DSA_Sig sig, DataStream msg);

DSAMaster create_default_master();
