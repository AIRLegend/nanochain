#ifndef NANOCHAIN_WALLET_H
#define NANOCHAIN_WALLET_H

#include "core/serializing.h"
#include "core/sign.h"
#include "core/peer.h"

#include "wallet/config.h"


class Wallet
{
public:
    Wallet(std::unique_ptr<Creedentials> creeds,
           std::vector<NodePeer>& peers);

    void save(std::string& path);
    void load(std::string& path);

    bool pay(std::string& to_addr, int amount);

    std::unique_ptr<Creedentials> m_creeds;
    std::vector<NodePeer> m_nodes;

private:
    Transaction buildTransaction(std::string& to_addr, int amount);
    bool broadcast(Transaction& tx);
};

#endif // NANOCHAIN_WALLET_H