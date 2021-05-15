#include "wallet.h"

#include "wallet/config.h"

#include "core/serializing.h"
#include "core/netmessaging.h"
#include "core/netclient.h"

#include "spdlog/sinks/null_sink.h"

using json = nlohmann::json;

Wallet::Wallet(std::unique_ptr<Creedentials> creeds,
               std::vector<NodePeer>& peers,
               std::shared_ptr<spdlog::logger> logger):
    m_nodes(peers)
{
    m_creeds = std::move(creeds);
    m_logger = logger;
}

bool Wallet::pay(std::string& to_addr, int amount) 
{
    Transaction t = buildTransaction(to_addr, amount);
    bool success = broadcast(t);
    return true;
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

bool Wallet::broadcast(const Transaction& tx) 
{
    networking::SenderClient client;
    for(NodePeer peer: m_nodes) 
    {
        std::string tx_repr = transactionToJSON(tx).dump();

        networking::NetResponse resp = client.send(tx_repr, 
                                                   networking::OP_TYPE::OP_BROADCAST_TX, 
                                                   peer.address);
        if (resp.status == networking::MESSAGE_STATUS::BAD) {
            m_logger->error("Got Net error with peer: " + peer.address);
        }
    }
    return true;
}