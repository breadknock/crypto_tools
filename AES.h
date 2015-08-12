#include <vector>
#include "DataStream.h"

#ifndef __aes__h
#define __aes__h

DataStream aes128_encrypt_block(DataStream key, DataStream block);
DataStream aes128_decrypt_block(DataStream key, DataStream block);
DataStream pad(const DataStream &stream);

DataStream aes128_encrypt_ecb(const DataStream &key, const DataStream &dec);
DataStream aes128_decrypt_ecb(const DataStream &key, const DataStream &enc);
DataStream aes128_encrypt_cbc(const DataStream &key, DataStream iv, const DataStream &dec);
DataStream aes128_cbc_mac(const DataStream &key, DataStream iv, const DataStream &dec);
DataStream aes128_decrypt_cbc(const DataStream &key, DataStream iv, const DataStream &enc);
DataStream aes128_encrypt_ctr(const DataStream &key, const DataStream &nonce, const DataStream &dec);
void aes128_edit_ctr(const DataStream &key, const DataStream &nonce, DataStream &enc, const DataStream &new_text, unsigned long offset);
DataStream aes128_decrypt_ctr(const DataStream &key, const DataStream &nonce, const DataStream &enc);
#endif
