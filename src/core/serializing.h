#ifndef NANOCHAIN_SERIALIZING_H
#define NANOCHAIN_SERIALIZING_H

#include "json.hpp"

#include "core/transaction.h"
#include "core/block.h"

using json = nlohmann::json;

// Parsing to JSON format
json transactionToJSON(const Transaction& tx);
json transactionsToJSON(const std::vector<Transaction> &txs);
json blockToJSON(const Block& block);
json blocksToJSON(const std::vector<Block> &blocks);

// Recovering from JSON format
Transaction transactionFromJSON(const json& obj);
Block blockFromJSON(const json& obj);

// IO
void toDisk(const std::string& path, const json& obj);
json fromDisk(const std::string& path);

// Utils for parsing byte arrays (hashes and signatures) to strings
std::string bytesToString(const unsigned char* bytes, size_t len=HASH_SIZE);
void stringToBytes(const std::string& hash_str, unsigned char* out);

#endif //NANOCHAIN_SERIALIZING_H