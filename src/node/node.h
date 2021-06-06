#ifndef NANOCHAIN_NODE_H
#define NANOCHAIN_NODE_H

#include <memory>
#include <unordered_map>

#include "core/block.h"
#include "transaction_pool.h"
#include "block_pool.h"
#include "core/peer.h"
#include "nodeconfig.h"

#include "json.hpp"

#include "listen_server.h"

#include "spdlog/spdlog.h"


using json = nlohmann::json;

/**
 * This class represents a node on the network. 
 * Each node functions both as a client and as a server.
 * 
 * Client functionalities:
 *  - Talk to other nodes, syncing info
 * Server functionalities:
 *  - Providing info to other client/nodes
 *  - Accepting transactions made from wallets
 *  - Accepting new blocks sent by miners
 */
class Node : public IServerSub
{
public:

    Node(const NodeConfig& conf, std::shared_ptr<spdlog::logger> logger);
    ~Node();

    /**
     * Adds a block to the chain (if it is valid).
     * 
     */ 
    bool addBlock(const Block &blck);

    /**
     * Adds a peer to the list of known nodes. Each peer should be 
     * a node on the network.
     */
    void registerPeer(NodePeer& peer);

    //Client methods (for talking to other nodes)
    /**
     * Requests all blocks of the known peers and adds the blocks 
     * to the internal chain (checking they are valid). 
     */
    void requestBlocks();
    /**
     * Request the mempool of other nodes.
     */ 
    void requestTxs();


    /**
    * Server responses callbacks. (IServerSub implementations)
    */
    void onNewBlock(const json& new_block, networking::NetResponse& response) override;
    void onBlockRequest(const json& block_hash, networking::NetResponse& response) override;
    void onNewTx(const json& new_tx, networking::NetResponse& response) override;
    void onTxRequest(networking::NetResponse& response) override;
    void onBalanceRequest(const json& address, networking::NetResponse& response) override;

private:
    /**
     * Finds a block by hash on the chain.
     */
    const std::shared_ptr<Block> findBlock(const unsigned char *hash);

    std::vector<NodePeer> m_peers;

    std::shared_ptr<spdlog::logger> m_logger;

    std::unordered_map<std::string, int> m_balances;

    BlockPool m_blockpool;

    // This is the mempool. All txs which have not been processed yet.
    TransactionPool m_txpool;
    // The blockchain itself.
    std::vector<Block> m_chain;

    /**
     * Clears all the mempool transactions which are present on the block.
     * This is called on each block addition, so miners don't fetch the same transacion 
     * over and over.
     */
    void filterMempool(Block& block);

    /**
     * Reads the entire blockchain updating the balance of all accounts. 
     */
    void updateBalances();

    /**
     * Returns whether a block is valid.
     * A block is valid if:
     *  - The previous hash exist on the chain
     *  - All transactions are valid (all come from addresses with enough balance)
     *  - TODO: Extra checks...
     * 
     * @param blck. Block to check
     * @return true if the blck is valid, false if not. 
     */
    bool validateBlock(const Block &blck);
};


#endif //NANOCHAIN_NODE_H