#include <boost/multiprecision/cpp_int.hpp>
#include "DataStream.h"
using boost::multiprecision::cpp_int;

class DH_Client {
private:
    cpp_int p;
    cpp_int base;
    cpp_int private_key;
    cpp_int public_key;
public:
    DH_Client();
    DH_Client(const cpp_int &p, const cpp_int &base);

    cpp_int get_base() { return base; }
    cpp_int get_prime() { return p; }
    cpp_int get_public_key();
    bool verify_private_key(const cpp_int &guess);
    cpp_int get_shared_secret(const cpp_int &other);
    DataStream get_shared_secret_data(const cpp_int &other);
};
