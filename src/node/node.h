#ifndef NANOCHAIN_NODE_H
#define NANOCHAIN_NODE_H

#include <memory>

#include "core/block.h"
#include "transaction_pool.h"
#include "block_pool.h"
#include "node/peer.h"

#include "json.hpp"

#include "listen_server.h"

using json = nlohmann::json;

class Node : public IServerSub
{
public:

    Node();
    ~Node();

    bool validateBlock(const Block &blck);
    bool addBlock(const Block &blck);

    BlockPool m_blockpool;
    TransactionPool m_txpool;
    std::vector<Block> m_chain;

    std::vector<NodePeer> m_peers;
    
    /**
    * Client methods (for talking to other nodes)
    */
    //Requests all the blocks the peers have
    void requestBlocks();
    // Asks for the mempool of other nodes
    void requestTxs();


    /**
    * Server responses callbacks. (IServerSub)
    */
    void onNewBlock(const std::string new_block, networking::NetResponse& response) override;
    void onBlockRequest(const std::string block_hash, networking::NetResponse& response) override;
    void onNewTx(const std::string new_tx, networking::NetResponse& response) override;
    void onTxRequest(networking::NetResponse& response) override;

private:
    const std::shared_ptr<Block> findBlock(const unsigned char *hash);
};


#endif //NANOCHAIN_NODE_H