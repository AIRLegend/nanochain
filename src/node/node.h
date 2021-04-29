#ifndef NANOCHAIN_NODE_H
#define NANOCHAIN_NODE_H

#include <memory>

#include "core/block.h"
#include "transaction_pool.h"
#include "block_pool.h"


#include "json.hpp"

#include "listen_server.h"

using json = nlohmann::json;

class Node : public IServerSub
{
public:

    Node();
    ~Node();

    bool validateBlock(const Block &blck);

    std::shared_ptr<BlockPool> pool;
    std::shared_ptr<TransactionPool> pooltx;
    std::vector<Block> chain;

     // Server responses callbacks. (IServerSub)
    void onNewBlock(const std::string new_block, networking::NetResponse& response) override;
    void onBlockRequest(const std::string block_hash, networking::NetResponse& response) override;
    void onNewTx(const std::string new_tx, networking::NetResponse& response) override;
    void onTxRequest(networking::NetResponse& response) override;

private:
    const std::shared_ptr<Block> findBlock(const unsigned char *hash);
};


#endif //NANOCHAIN_NODE_H