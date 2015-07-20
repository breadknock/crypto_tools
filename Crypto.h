#include <vector>
#include "DataStream.h"

#ifndef __crypto__h
#define __crypto__h

DataStream aes128_encrypt_ecb(const DataStream &key, const DataStream &dec);
DataStream aes128_decrypt_ecb(const DataStream &key, const DataStream &enc);
DataStream aes128_encrypt_cbc(const DataStream &key, DataStream iv, const DataStream &dec);
DataStream aes128_decrypt_cbc(const DataStream &key, DataStream iv, const DataStream &enc);

DataStream pad(const DataStream &data);

bool check_padding(const DataStream &data);

DataStream get_random_key(int length = 16);

#endif