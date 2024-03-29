#include "node.h"

#include "core/block.h"
#include "core/serializing.h"
#include "core/netclient.h"
#include "core/netmessaging.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/null_sink.h"

Node::Node(const NodeConfig& conf, std::shared_ptr<spdlog::logger> logger) {
    m_logger = logger;
}

Node::~Node() {}

const std::shared_ptr<Block> Node::findBlock(const unsigned char *hash)
{
    for(int i=m_chain.size()-1; i>=0; i--) {
        Block &blocki = m_chain.at(i);
        int comp = memcmp(blocki.b_hash, hash, HASH_SIZE);
        if ( comp == 0)
            return std::make_shared<Block>(blocki);
    }

    return nullptr;
}

void Node::filterMempool(Block& block) 
{
    for (Transaction& tx : block.txs) {
        if(m_txpool.remove(tx))
            m_logger->debug("TX: " + bytesToString(tx.m_from) + " removed from mempool as it has been included in a block.");
    }
}

bool Node::validateBlock(const Block &blck) {
    //std::shared_ptr<Block> prevbBockMatch = findBlock(blck.prev_hash);
    //if (prevbBockMatch == nullptr)
    //    return false;

    // Matching block found
    // TODO: Check blck data

    // If block is valid
    // Remove block transactions from pool
    return true;
}

bool Node::addBlock(const Block &blck) {
    bool is_valid = validateBlock(blck);
    if(is_valid)
        m_chain.push_back(blck);
    else
        return false;

    m_logger->info("New block "+ bytesToString(blck.b_hash) + " added to the chain");
    return true;
}

void Node::registerPeer(NodePeer& peer) {
    for(NodePeer& p : m_peers) {
        if(p.address.compare(peer.address) == 0)
            return;
    }
    this->m_peers.push_back(peer);
}


// Client methods
void Node::requestBlocks() 
{
    networking::SenderClient client;

    for (NodePeer& peer: m_peers) {
        json payload = {
            {"hash", ""}
        };
        networking::NetResponse resp = client.send(payload.dump(), networking::OP_TYPE::OP_BLOCK_REQ, peer.address);
        
        for (json j : resp.data) {
            Block b = blockFromJSON(j);
            addBlock(b);
        }

    }
}

void Node::requestTxs() 
{
    networking::SenderClient client;

    for (NodePeer& peer: m_peers) {
        networking::NetResponse resp = client.send("{}", networking::OP_TYPE::OP_MEMPOOL_REQUEST, peer.address);
        for (json j : resp.data) {
            Transaction t = transactionFromJSON(j);
            m_txpool.add(t);

            m_logger->info("New TX added to the POOL. From: " + bytesToString(t.m_from, 8) + 
                            " To: " + bytesToString(t.m_to, 8) + 
                            " Amount: " + std::to_string(t.getAmount()));
        }
        
    }
}






// Server sub callbacks
void Node::onNewBlock(const json& new_block, networking::NetResponse& response) {
    //json blk_json = json::parse(new_block);
    Block b = blockFromJSON(new_block);

    // Attempt adding candiate block to the chain
    bool success = addBlock(b);

    if (!success) {
        m_logger->info("Block " + bytesToString(b.b_hash) + " is not valid.");
        response.status = networking::MESSAGE_STATUS::BAD;
        return;
    }
    
    // Remove transactions from the mempool
    filterMempool(b);

    m_logger->info("Block " + bytesToString(b.b_hash) + "added to the chain. Size: " +  std::to_string(m_chain.size()));
    response.status = networking::MESSAGE_STATUS::OK;
}

void Node::onBlockRequest(const json& block_hash, networking::NetResponse& response) {
    std::string hashblk = block_hash["hash"];

    if (hashblk.size() == 0) {
        // Empty hash -> Return all blocks
        // TODO: This is crazy, for real usage this should be removed.
        response.data = blocksToJSON(this->m_chain);
        response.status = networking::MESSAGE_STATUS::OK;
    } else if(hashblk.compare("-1") == 0){
        // Return only the latest accepted block
        Block b = m_chain.back();
        response.data = blockToJSON(b);
        response.status = networking::MESSAGE_STATUS::OK;
    } else {
        unsigned char hashbl[HASH_SIZE];
        stringToBytes(hashblk, hashbl);
        auto blk = findBlock(hashbl);
        // TODO: Check nullptr
        response.data = blockToJSON(*blk);
        response.status = networking::MESSAGE_STATUS::OK;
    }
}

void Node::onNewTx(const json& newTx, networking::NetResponse& response) {
    // TODO: Implement
    // Check everything is correct

    // Add to the mempool
    Transaction tx = transactionFromJSON(newTx);
    m_txpool.add(tx);
    m_logger->info("New TX added to the POOL. From: " + bytesToString(tx.m_from, 16) + 
                    " To: " + bytesToString(tx.m_to, 16) + 
                    " Amount: " + std::to_string(tx.getAmount()));

    response.status = networking::MESSAGE_STATUS::OK;
    response.data = true;
}

void Node::onTxRequest(networking::NetResponse& response) {
    // TODO: Return the entire mempool
    response.data = transactionsToJSON(m_txpool.getTxs());
    response.status = networking::MESSAGE_STATUS::OK;
}