#include "core/serializing.h"

#include <sstream>

json transactionToJSON(const Transaction& tx)
{
    json repr = {
            {"from", bytesToString(tx.m_from, SIGNATURE_LEN)},
            {"to", bytesToString(tx.m_to, SIGNATURE_LEN)},
            {"amount", tx.getAmount()},
            {"signature", bytesToString(tx.m_signature, SIGNATURE_LEN)},
    };

    return repr;
}

json transactionsToJSON(const std::vector<Transaction> &txs)
{
    std::vector<json> reprs;

    for (Transaction tx: txs) {
        reprs.push_back(transactionToJSON(tx));
    }
    
    return reprs;
}

json blockToJSON(const Block& block)
{
    std::string  prevHash = bytesToString(block.prev_hash);
    std::string  blockHash = bytesToString(block.b_hash);

    auto txs = json::array();

    for (Transaction tx:block.txs)
        txs.push_back(transactionToJSON(tx));

    json repr = {
            {"block_hash", blockHash},
            {"prev_hash", prevHash},
            {"txs", txs}
    };

    return repr;
}

json blocksToJSON(const std::vector<Block> &blocks)
{
    std::vector<json> reprs;

    for (Block bl: blocks) {
        reprs.push_back(blockToJSON(bl));
    }

    return reprs;
}


// Recovering from JSON format
Transaction transactionFromJSON(const json& obj)
{
    std::string from_str = obj["from"].get<std::string>();
    std::string to_str = obj["to"].get<std::string>();
    std::string signature_str = obj["signature"].get<std::string>();
    int amount = obj["amount"].get<int>();

    unsigned char from[SIGNATURE_LEN];
    unsigned char to[SIGNATURE_LEN];
    stringToBytes(from_str, from);
    stringToBytes(to_str, to);

    Transaction tx(from, to, amount=amount);
    stringToBytes(signature_str, tx.m_signature);
    return tx;
}

Block blockFromJSON(const json& obj)
{
    std::string bhash = obj["block_hash"].get<std::string>();
    std::string phash = obj["prev_hash"].get<std::string>();

    Block b;

    stringToBytes(phash, b.prev_hash);
    stringToBytes(bhash, b.b_hash);

    for (json json_t:obj["txs"]) {
        b.txs.push_back(transactionFromJSON(json_t));
    }

    return b;
}


// IO
void toDisk(const std::string& path, const json& obj)
{
    std::ofstream o(path);
    o << std::setw(4) << obj << std::endl;
}

json fromDisk(const std::string& path)
{
    std::ifstream i(path);
    json j;
    i >> j;
    return j;
}


// Utils for parsing byte arrays (hashes and signatures) to strings
std::string bytesToString(const unsigned char* bytes, size_t len)
{
    std::stringstream hashStr;
    for (int i=0; i<len; i++) {
        hashStr << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(bytes[i]);
    }
    return hashStr.str();
}

void stringToBytes(const std::string& hash_str, unsigned char* out)
{
    for (int i=0; i<hash_str.size(); i+=2) {
        std::istringstream converter(hash_str.substr(i, 2));
        unsigned int value;
        converter >>  std::hex >> value;
        out[int(i/2)] = static_cast<unsigned char>(value);
    }
}
