#include "miner/miner.h"
#include "core/netclient.h"
#include "core/serializing.h"

#include "spdlog/spdlog.h"

//#include <bits/stdc++.h>
#include <ctime>


Miner::Miner() {
    memset(m_cmp_zeros_buffer, 0, HASH_SIZE);
}

void Miner::addPeer(NodePeer& peer) {
    m_peers.push_back(peer);
}

void Miner::requestMempool() {
    networking::SenderClient client;
    for (NodePeer& peer: m_peers) {
        networking::NetResponse resp = client.send("{}", networking::OP_TYPE::OP_MEMPOOL_REQUEST, peer.address);
        for (json j : resp.data) {
            Transaction t = transactionFromJSON(j);
            m_mempool.add(t);
            spdlog::get("nanominer")->info("New TX added to the POOL");
        }
    }
}

void Miner::startMining()  {
    if(!m_should_mine) {
        m_should_mine = true;
        miner_thread = std::make_unique<std::thread>(&Miner::mine, this);    
        miner_thread->detach();
    }
}

void Miner::stopMining() 
{
    m_should_mine = false;
    auto h = miner_thread->native_handle();
    pthread_cancel(h);
    miner_thread.reset();
    spdlog::get("nanominer")->info("Mining ended.");
}

void Miner::broadcastBlock() 
{
    auto logger = spdlog::get("nanominer");
    logger->info("Broadcasting current candidate block...");

    std::string json_block = blockToJSON(m_candidate).dump();
    
    networking::SenderClient client;

    for(NodePeer peer:m_peers) {
        logger->info("Sent to peer: " + peer.address);
        client.send(json_block, networking::OP_TYPE::OP_BLOCK_ANNOUNCE, peer.address);
        // TODO: Handle bad request (block invalid or whatever)
    }
}

void Miner::mine()  {
    auto logger = spdlog::get("nanominer");

    unsigned char current_hash[HASH_SIZE];
    for(int nonce=INT_MIN; nonce<INT_MAX; nonce++) {
        m_candidate.nonce = nonce;
        sha256(&m_candidate, current_hash, sizeof(m_candidate));
        logger->info("Generated hash: " + bytesToString(current_hash));
        bool validHash = memcmp(current_hash, m_cmp_zeros_buffer, m_curr_difficulty) == 0;
        if (validHash)
            break;
    }
    logger->info("Found valid hash! " + bytesToString(current_hash));
    memcpy(m_candidate.b_hash, current_hash, HASH_SIZE);
    broadcastBlock();
    m_mempool.flush();
    m_should_mine = false;
    spdlog::get("nanominer")->info("Mining ended.");
}

void Miner::buildCandidateBlock()  {
    m_candidate.creation_time = std::time(nullptr);

    // Get 10 transactions  from the mempool
    for (int i=0; i<std::min(10, (int)m_mempool.m_txs.size()); i++) {
        m_candidate.txs.push_back(m_mempool.m_txs.at(i));
    }
    
    spdlog::get("nanominer")->info("Candidate block built");
}