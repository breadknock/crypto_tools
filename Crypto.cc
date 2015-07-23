#include "Crypto.h"
#include "DataStream.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

// Thanks, Wikipedia!

unsigned char s[256] = 
{
   0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
   0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
   0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
   0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
   0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
   0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
   0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
   0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
   0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
   0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
   0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
   0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
   0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
   0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
   0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
   0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

unsigned char inv_s[256] = 
{
   0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
   0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
   0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
   0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
   0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
   0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
   0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
   0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
   0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
   0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
   0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
   0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
   0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
   0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
   0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
   0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D
};

unsigned char iSbox(unsigned char c) {
    return inv_s[c];
}

unsigned char sbox(unsigned char c) {
    return s[c];
}

unsigned char gmul(unsigned char c, unsigned char c2) {
    unsigned char result = 0;
    while(c2) {
        if(c2&0x1) {
            result ^= c;
        }
        c = (c << 1) ^ ((c&0x80)?0x1b:0);
        c2 >>= 1;
    }
    return result;
}

//multiply a square matrix by a rectangular matrix
std::vector<unsigned char> gmmul(std::vector<unsigned char> a, 
                                 std::vector<std::vector<unsigned char> > b) {
    std::vector<unsigned char> ans;
    for(unsigned int i = 0; i < b[0].size(); i++) {
        unsigned char result = 0;
        for(unsigned int j = 0; j < a.size(); j++) {
            result ^= gmul(a[j],b[i][j]);
        }
        ans.push_back(result);
    }
    return ans;
}

DataStream invMixColumns(const DataStream &ds) {
    std::vector<std::vector<unsigned char> > mat;
    for(int i = 0; i < 4; i++) {
        std::vector<unsigned char> row(4);
        row[(i+0)%4] = 0xe;
        row[(i+1)%4] = 0xb;
        row[(i+2)%4] = 0xd;
        row[(i+3)%4] = 0x9;
        mat.push_back(row);
    }
    std::vector<DataStream> columns = ds.chunk(4);
    DataStream result;
    for(int i = 0; i < 4; i++) {
        result.append(DataStream(gmmul(columns[i].get_data(),mat)));
    }
    return result;
}

DataStream mixColumns(const DataStream &ds) {
    std::vector<std::vector<unsigned char> > mat;
    for(int i = 0; i < 4; i++) {
        std::vector<unsigned char> row(4);
        row[(i+0)%4] = 2;
        row[(i+1)%4] = 3;
        row[(i+2)%4] = 1;
        row[(i+3)%4] = 1;
        mat.push_back(row);
    }
    std::vector<DataStream> columns = ds.chunk(4);
    DataStream result;
    for(int i = 0; i < 4; i++) {
        result.append(DataStream(gmmul(columns[i].get_data(),mat)));
    }
    return result;
}

DataStream rcon(int x) {
    std::vector<unsigned char> data;
    if(x < 8) {
        data.push_back(1<<x);
    } else if(x == 8) {
        data.push_back(0x1b);
    } else if(x == 9) {
        data.push_back(0x36);
    }
    for(int i = 0; i < 3; i++) {
        data.push_back(0);
    }
    return DataStream(data);
}

DataStream invSubBytes(const DataStream &ds) {
    std::vector<unsigned char> data;
    for(const unsigned char &c : ds.get_data()) {
        
        data.push_back(iSbox(c));
    }
    return DataStream(data);
}

DataStream subBytes(const DataStream &ds) {
    std::vector<unsigned char> data;
    for(unsigned char c : ds.get_data()) {
        data.push_back(sbox(c));
    }
    return DataStream(data);
}

DataStream invShiftRows(const DataStream &ds) {
    std::vector<DataStream> words = ds.partition(4);
    std::vector<std::vector<unsigned char> > result_rows;
    for(int i = 0; i < 4; i++) {
        result_rows.push_back(words[i].rotate(4-i).get_data());
    }
    std::vector<unsigned char> result;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            result.push_back(result_rows[j][i]);
        }
    }
    return DataStream(result);
    
}
//Do stuff
DataStream shiftRows(const DataStream &ds) {
    std::vector<DataStream> words = ds.partition(4);
    std::vector<std::vector<unsigned char> > result_rows;
    for(int i = 0; i < 4; i++) {
        result_rows.push_back(words[i].rotate(i).get_data());
    }
    std::vector<unsigned char> result;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            result.push_back(result_rows[j][i]);
        }
    }
    return DataStream(result);
}


std::vector<DataStream> aes_expand_key(DataStream key) {
    std::vector<DataStream> words = key.chunk(4);
    for(int i = 4; i < 44; i++) {
        if(i%4==0) {
            words.push_back(subBytes(words[i-1].rotate(1))^words[i-4]^rcon(i/4-1));
        } else {
            words.push_back(words[i-4]^words[i-1]);
        }
    }
    std::vector<DataStream> keys;
    for(int i = 0; i <= 10; i++) {
        DataStream full_key;
        for(int j = 0; j < 4; j++) {
            full_key.append(words[4*i+j]);
        }
        full_key.setInfinite();
        keys.push_back(full_key);
    }
    return keys;
}

DataStream addRoundKey(DataStream block, DataStream key) {
    return block ^ key;
}

DataStream pad(const DataStream &stream) {
    unsigned char pad = 16 - stream.get_size()%16;
    std::vector<unsigned char> data = stream.get_data();
    for(int i = 0; i < pad; i++) {
        data.push_back(pad);
    }   
    return DataStream(data);
}

bool check_pad(const DataStream &stream) {
    std::vector<unsigned char> data = stream.get_data();
    unsigned char length = data.back();
    if(length == 0) {
        return false;
    }
    for(int i = 0; i < length; i++) {
        if(data[data.size()-i-1] != length) {
            return false;
        }
    }
    return true;
}
DataStream unpad(const DataStream &stream) {
    std::vector<unsigned char> data = stream.get_data();
    unsigned char length = data.back();
    return DataStream(std::vector<unsigned char>(data.begin(), data.end()-length));
}

DataStream aes128_encrypt_block(DataStream key, DataStream block) {
    std::vector<DataStream> key_exp = aes_expand_key(key);
    block = addRoundKey(block, key_exp[0]);
    for(int i = 0; i < 9; i++) {
        block = mixColumns(shiftRows(subBytes(block)));
        block = addRoundKey(block, key_exp[i+1]);
    }
    block = shiftRows(subBytes(block));
    block = addRoundKey(block,key_exp[10]);
    return block;
}

//Do above, but backwards
DataStream aes128_decrypt_block(DataStream key, DataStream block) {
    std::vector<DataStream> key_exp = aes_expand_key(key);
    block = addRoundKey(block,key_exp[10]);
    block = invSubBytes(invShiftRows(block));
    for(int i = 9; i >= 1; i--) {
        block = addRoundKey(block, key_exp[i]);
        block = invSubBytes(invShiftRows(invMixColumns(block)));
    }
    block = addRoundKey(block, key_exp[0]);
    return block;
}

DataStream aes128_encrypt_ecb(const DataStream &key, const DataStream &dec) {
    std::vector<DataStream> chunks = pad(dec).chunk(16);
    DataStream enc;
    for(const DataStream &ds : chunks) {
        enc.append(aes128_encrypt_block(key,ds));
    }
    return enc;
}


DataStream aes128_decrypt_ecb(const DataStream &key, const DataStream &enc) {
    std::vector<DataStream> chunks = enc.chunk(16);
    DataStream dec;
    for(const DataStream &ds : chunks) {
        dec.append(aes128_decrypt_block(key,ds));
    }
    return dec;
}

DataStream aes128_encrypt_cbc(const DataStream &key, DataStream iv, const DataStream &dec) {
    std::vector<DataStream> chunks = pad(dec).chunk(16);
    DataStream enc;
    for(const DataStream &ds : chunks) {
        DataStream enc_block = (aes128_encrypt_block(key,ds^iv));
        enc.append(enc_block);
        iv = enc_block;
    }
    return enc;
}

DataStream aes128_decrypt_cbc(const DataStream &key, DataStream iv, const DataStream &enc) {
    std::vector<DataStream> chunks = enc.chunk(16);
    DataStream dec;
    for(const DataStream &ds : chunks) {
        DataStream dec_block = iv^aes128_decrypt_block(key,ds);
        dec.append(dec_block);
        iv = ds;
    }
    if(!check_pad(dec)) {
        throw 1;
    }
    return unpad(dec);
}

DataStream aes128_encrypt_ctr(const DataStream &key, const DataStream &nonce, const DataStream &dec) {
    std::vector<DataStream> chunks = dec.chunk(16);
    DataStream enc;
    for(unsigned long i = 0; i < chunks.size(); i++) {
        std::vector<unsigned char> ctr_data = nonce.get_data();
        for(int j = 0; j < 8; j++) {
            ctr_data.push_back((i>>(8*j))&0xff);
        }
        DataStream xor_block = aes128_encrypt_block(key,DataStream(ctr_data));
        enc.append(xor_block ^ chunks[i]);
    }
    return enc;

}

void aes128_edit_ctr(const DataStream &key, const DataStream &nonce, DataStream &enc, const DataStream &new_text, unsigned long offset) {
    std::vector<unsigned char> enc_data = enc.get_data();
    std::vector<unsigned char> new_data = new_text.get_data();
    for(unsigned long i = offset / 16; i <= (offset - 1 + new_data.size())/16; i++) {
        std::cout<<i<<std::endl;
        std::vector<unsigned char> ctr_data = nonce.get_data();
        for(int j = 0; j < 8; j++) {
            ctr_data.push_back((i>>(8*j))&0xff);
        }
        std::vector<unsigned char> xor_block = aes128_encrypt_block(key,DataStream(ctr_data)).get_data();
        unsigned long start = std::max((i*16),offset);
        unsigned long end = std::min((i+1)*16,offset + new_text.get_size());
        for(;start < end; start++) {
            enc_data[start] = xor_block[start%16] ^ new_data[start - offset];
        }
    }
    enc = DataStream(enc_data);
}

DataStream aes128_decrypt_ctr(const DataStream &key, const DataStream &nonce, const DataStream &enc) {
    std::vector<DataStream> chunks = enc.chunk(16);
    DataStream dec;
    for(unsigned long i = 0; i < chunks.size(); i++) {
        std::vector<unsigned char> ctr_data = nonce.get_data();
        for(int j = 0; j < 8; j++) {
            ctr_data.push_back((i>>(8*j))&0xff);
        }
        DataStream xor_block = aes128_encrypt_block(key,DataStream(ctr_data));
        dec.append(xor_block ^ chunks[i]);
    }
    return dec;
}

DataStream get_random_key(int length) {
    std::ifstream rand_file("/dev/urandom",std::ios::binary);
    unsigned char* data = new unsigned char[length];
    rand_file.read((char*)data, length);
    DataStream ds(std::vector<unsigned char>(data,data + length));
    delete[] data;
    rand_file.close();
    return ds;
}

