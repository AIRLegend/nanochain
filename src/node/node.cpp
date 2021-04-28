#include "node.h"

#include "core/block.h"
#include "core/serializing.h"

#include "spdlog/spdlog.h"

Node::Node() {}

Node::~Node() {}

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


// Server sub callbacks
void Node::onNewBlock(const std::string new_block, networking::NetResponse& response) {
    json blk_json = json::parse(new_block);
    Block b = blockFromJSON(blk_json);

    // Check whether new block is valid

    this->chain.push_back(b);
    std::cout << "NEW BLOCK ADDED TO THE CHAIN! - LENGTH=" << chain.size()  << std::endl;
    response.status = networking::MESSAGE_STATUS::OK;
}

void Node::onBlockRequest(const std::string blockHash, networking::NetResponse& response) {
    if (blockHash.size() == 0) {
        response.data = blocksToJSON(this->chain);
        response.status = networking::MESSAGE_STATUS::OK;
    } else {
        unsigned char hashbl[HASH_SIZE];
        stringToBytes(blockHash, hashbl);
        auto blk = findBlock(hashbl);
        // TODO: Check nullptr
        response.data = blockToJSON(*blk);
        response.status = networking::MESSAGE_STATUS::OK;
    }
}