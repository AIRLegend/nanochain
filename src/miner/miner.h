#ifndef NANOCHAIN_MINER_H
#define NANOCHAIN_MINER_H

#include "core/peer.h"
#include "miner/mempool.h"
#include "core/hashing.h"

#include <memory>
#include <thread>
#include <vector>

/**
 * Nanochain miner containing logic from fetching mempool transactions,
 * building blocks and broadcasting them to the network.
 */
class Miner
{
public:
    Mempool m_mempool;

    Miner();

    /**
     * Adds a node to the list of known nodes.
     * Blocks will be announced to all of them,
     */ 
    void addPeer(NodePeer& peer);
    
    /**
     * Fetches all the transactions of the nodes' mempools.
     */
    void requestMempool();

    /**
     * Request the latest block hash. We dont need to know the entire block.
     * TODO: Probably bad because of someone could send us a wrong hash. We need the 
     * entire block for checking it is okay
     */
    void requestLatestHash();

    /**
     * Spawns a new thread with the mining process which 
     * consists on changing the nonce until  it finds a valid hash.
     * If it's already busy mining it will do nothing.
     */
    void startMining();

    /**
     * Forces the mining to stop (killing the thread)
     */
    void stopMining();

    /**
     * Announce the current found new block (m_candidate) to all known peers 
     */
    void broadcastBlock();


private:
    std::vector<NodePeer> m_peers;
    std::unique_ptr<std::thread> miner_thread;
    
    unsigned char m_cmp_zeros_buffer[HASH_SIZE];
    int m_curr_difficulty = 1;
    bool m_should_mine = false;

    Block m_candidate;
    
    /**
     * Loads the m_candidate with the data we will include for the 
     * next announcement.
     */
    void buildCandidateBlock();

    /**
     * Runs the mining loop
     */
    void mine();
};

#endif //NANOCHAIN_MINER_H