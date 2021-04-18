#include "node.h"

const std::shared_ptr<Block> Node::findBlock(const unsigned char *hash)
{
    for(int i=chain.size()-1; i>=0; i--) {
        Block &blocki = chain.at(i);
        int comp = memcmp(blocki.b_hash, hash, HASH_SIZE);
        if ( comp == 0)
            return std::make_shared<Block>(blocki);
    }

    return nullptr;
}

bool Node::validateBlock(const Block &blck) {
    std::shared_ptr<Block> prevbBockMatch = findBlock(blck.prev_hash);
    if (prevbBockMatch == nullptr)
        return false;

    // Matching block found
    // TODO: Check blck data

    // If block is valid
    // Remove block transactions from pool
    return true;
}