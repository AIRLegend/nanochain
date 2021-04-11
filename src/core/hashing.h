
#ifndef NANOCHAIN_HASHING_H
#define NANOCHAIN_HASHING_H

#include <cstdio>
#include <memory>
#include "sha.h"


const int HASH_SIZE =  CryptoPP::SHA256::DIGESTSIZE;

void sha256(void* in, unsigned char* out, size_t len);
std::string sha2str(const unsigned char* hash);
void str2sha(const std::string& hash_str, unsigned char* out);


#endif //NANOCHAIN_HASHING_H
