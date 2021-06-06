#ifndef NANOCHAIN_WALLET_H
#define NANOCHAIN_WALLET_H

#include "core/serializing.h"
#include "core/sign.h"
#include "core/peer.h"

#include "wallet/config.h"

#include "spdlog/spdlog.h"


/**
 * Simple wallet providing control over a network's address.
 * It allows for checking the account's balance and broadcasting 
 * new transactions to the network.
 */
class Wallet
{
public:
    Wallet(std::unique_ptr<Creedentials> creeds,
           std::vector<NodePeer>& peers,
           std::shared_ptr<spdlog::logger> logger);

    /**
     * Saves wallet creedentials (Pub and Priv keys) and state.
     */
    void save(std::string& path);

    /**
     * Loads wallet creedentials (Pub and Priv keys) and state.
     */
    void load(std::string& path);

    /**
     * Broadcast a new transaction
     */
    bool pay(std::string& to_addr, int amount);

    // Creedentials (priv and pub keys) for signing the transactions
    std::unique_ptr<Creedentials> m_creeds;

    // List of known nodes.
    std::vector<NodePeer> m_nodes;

private:
    Transaction buildTransaction(std::string& to_addr, int amount);

    /**
     * Sends the transaction over the network
     */
    bool broadcast(const Transaction& tx);

    std::shared_ptr<spdlog::logger> m_logger;
};

#endif // NANOCHAIN_WALLET_H