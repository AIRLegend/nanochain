#ifndef NANOCHAIN_MINER_H
#define NANOCHAIN_MINER_H

#include "core/peer.h"
#include "miner/mempool.h"
#include "core/hashing.h"

#include <memory>
#include <thread>
#include <vector>

class Miner
{
public:
    Mempool m_mempool;

    Miner();
    void addPeer(NodePeer& peer);
    void requestMempool();
    void startMining();
    void stopMining();
    void broadcastBlock();

    void mine();

private:
    std::vector<NodePeer> m_peers;
    std::unique_ptr<std::thread> miner_thread;
    
    unsigned char m_cmp_zeros_buffer[HASH_SIZE];
    int m_curr_difficulty = 2;
    bool m_should_mine = false;
    //std::mutex m_mine_mutex;

    Block m_candidate;


    
    void buildCandidateBlock();
};

#endif //NANOCHAIN_MINER_H