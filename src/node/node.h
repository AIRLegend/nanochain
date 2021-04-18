#ifndef NANOCHAIN_NODE_H
#define NANOCHAIN_NODE_H

#include <memory>

#include "core/block.h"
#include "transaction_pool.h"
#include "block_pool.h"

#include "json.hpp"

using json = nlohmann::json;

class Node 
{
public:
    bool validateBlock(const Block &blck);

    std::shared_ptr<BlockPool> pool;
    std::shared_ptr<TransactionPool> pool;
    std::vector<Block> chain;

private:
    const std::shared_ptr<Block> findBlock(const unsigned char *hash);
};


#endif //NANOCHAIN_NODE_H