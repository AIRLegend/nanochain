#ifndef NANOCHAIN_BLOCK_H
#define NANOCHAIN_BLOCK_H

#include <cstdint>
#include "core/transaction.h"
#include "core/hashing.h"

class Block
{
public:
    unsigned char b_hash[HASH_SIZE];
    unsigned char prev_hash[HASH_SIZE];
    std::vector<Transaction> txs;
    uint8_t num_tx = 0;
    unsigned long int creation_time;
    int nonce;

    /*void print(int len_hex = 30);
    std::string toString();
    std::string toJSON();
    friend std::ostream& operator<<(std::ostream& os, const block& dt);*/
};


#endif //NANOCHAIN_BLOCK_H