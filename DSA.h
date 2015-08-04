#include "DataStream.h"
#include <boost/multiprecision/cpp_int.hpp>
#include <utility>
using boost::multiprecision::cpp_int;
typedef std::pair<cpp_int, cpp_int> DSA_Sig;

struct DSA_key {
    cpp_int p;
    cpp_int q;
    cpp_int g;
    cpp_int y;
};

class DSAClient;

class DSAMaster {
private:
    cpp_int p;
    cpp_int q;
    cpp_int g;


public:
    DSAMaster(); //not implemented
    DSAMaster(const cpp_int &_p, const cpp_int &_q, const cpp_int &_g) : p(_p), q(_q), g(_g) {}
    DSAClient generate_client(cpp_int x = 0);
};

class DSAClient {
    friend class DSAMaster;
private:
    cpp_int p;
    cpp_int q;
    cpp_int g;
    cpp_int x;
    cpp_int y;
    DSAClient(const cpp_int &p, const cpp_int &q, const cpp_int &g, const cpp_int &x);
public:
    DSA_Sig sign(const DataStream &msg, cpp_int key = 0);
    DSA_key get_public_key();

};
bool verify(const DSA_key &pub_key, const DSA_Sig &sig, const DataStream &msg);

DSAMaster create_default_master();
