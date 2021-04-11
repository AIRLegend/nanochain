
#ifndef LIGHTCHAIN_HASH_H
#define LIGHTCHAIN_HASH_H


#include <cstdio>
#include <memory>
#include "sha.h"

class hash {
public:
    hash(void* data, size_t len);
    unsigned char hash_ptr[CryptoPP::SHA256::DIGESTSIZE];
};

const int HASH_SIZE =  CryptoPP::SHA256::DIGESTSIZE;

void sha256(void* in, unsigned char* out, size_t len);
std::string sha2str(const unsigned char* hash);
void str2sha(const std::string& hash_str, unsigned char* out);


#endif //LIGHTCHAIN_HASH_H
