#ifndef NANOCHAIN_WALLET_H
#define NANOCHAIN_WALLET_H

#include "core/serializing.h"
#include "core/sign.h"
#include "core/peer.h"

#include "wallet/config.h"

#include "spdlog/spdlog.h"


class Wallet
{
public:
    Wallet(std::unique_ptr<Creedentials> creeds,
           std::vector<NodePeer>& peers,
           std::shared_ptr<spdlog::logger> logger);

    void save(std::string& path);
    void load(std::string& path);

    bool pay(std::string& to_addr, int amount);

    std::unique_ptr<Creedentials> m_creeds;
    std::vector<NodePeer> m_nodes;

private:
    Transaction buildTransaction(std::string& to_addr, int amount);
    bool broadcast(const Transaction& tx);

    std::shared_ptr<spdlog::logger> m_logger;
};

#endif // NANOCHAIN_WALLET_H