#include "wallet.h"

#include "wallet/config.h"

#include "core/serializing.h"
#include "core/netmessaging.h"
#include "core/netclient.h"

using json = nlohmann::json;

Wallet::Wallet(std::unique_ptr<Creedentials> creeds,
               std::vector<NodePeer>& peers):
    m_nodes(peers)
{
    m_creeds = std::move(creeds);
}

bool Wallet::pay(std::string& to_addr, int amount) 
{
    Transaction t = buildTransaction(to_addr, amount);
    bool success = broadcast(t);
}

Transaction Wallet::buildTransaction(std::string& to_addr, int amount) 
{
    unsigned char bytes_addr[PubKey::PUBK_LEN];
    stringToBytes(to_addr, bytes_addr);
    Transaction t(m_creeds->m_keypair->publicKey->m_bytes,
                  bytes_addr,
                  amount);   

    // Sign the transaction with our PK
    m_creeds->m_keypair->privateKey->sign(t.m_signature, 
                                          (unsigned char*) &t, 
                                          sizeof(t));

    return t;
}

bool Wallet::broadcast(Transaction& tx) 
{
    for(NodePeer peer: m_nodes) 
    {
        
    }
}