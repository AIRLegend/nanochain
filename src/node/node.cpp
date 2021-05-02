#include "node.h"

#include "core/block.h"
#include "core/serializing.h"
#include "core/netclient.h"
#include "core/netmessaging.h"

#include "spdlog/spdlog.h"

Node::Node() {

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

    spdlog::get("console")->info("[NODE] New block "+ bytesToString(blck.b_hash) + " added to the chain");
    return true;
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
            spdlog::get("console")->info("[NODE] New TX added to the POOL");
        }
        
    }
}






// Server sub callbacks
void Node::onNewBlock(const std::string new_block, networking::NetResponse& response) {
    json blk_json = json::parse(new_block);
    Block b = blockFromJSON(blk_json);

    // Attempt adding candiate block to the chain
    bool success = addBlock(b);

    if (!success) {
        spdlog::get("console")->info("[NODE] Block " + bytesToString(b.b_hash) + " is not valid.");
        response.status = networking::MESSAGE_STATUS::BAD;
        return;
    }
    
    // TODO: Remove transactions from the mempool

    spdlog::get("console")->info("[NODE] New block added to the chain. Size: " +  m_chain.size());
    response.status = networking::MESSAGE_STATUS::OK;
}

void Node::onBlockRequest(const std::string block_hash, networking::NetResponse& response) {
    if (block_hash.size() == 0) {
        response.data = blocksToJSON(this->m_chain);
        response.status = networking::MESSAGE_STATUS::OK;
    } else {
        unsigned char hashbl[HASH_SIZE];
        stringToBytes(block_hash, hashbl);
        auto blk = findBlock(hashbl);
        // TODO: Check nullptr
        response.data = blockToJSON(*blk);
        response.status = networking::MESSAGE_STATUS::OK;
    }
}

void Node::onNewTx(const std::string newTx, networking::NetResponse& response) {
    // TODO: Implement
    // Check everything seems correct

    // Add to the mempool
    json json_tx = json::parse(newTx);
    m_txpool.add(transactionFromJSON(json_tx));
    response.status = networking::MESSAGE_STATUS::OK;
}

void Node::onTxRequest(networking::NetResponse& response) {
    // TODO: Return the entire mempool
    response.data = transactionsToJSON(m_txpool.getTxs());
    response.status = networking::MESSAGE_STATUS::OK;
}