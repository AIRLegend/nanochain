#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include "wallet/wallet.h"
#include "wallet/config.h"

#include "core/serializing.h"
#include "core/peer.h"

using json = nlohmann::json;

int main() {
    auto console = spdlog::stdout_color_mt("nanowallet");
    spdlog::get("nanowallet")->info("Welcome to the wallet.");
    std::shared_ptr<spdlog::logger> logger = spdlog::get("nanowallet");

    NodePeer peer("tcp://192.168.0.72:5557");
    std::vector peers = { peer };

    std::string creeds_path = "creeds.json";
    std::unique_ptr<Creedentials> creds = Creedentials::fromFile(creeds_path);

    std::string pubk_dest = "40819d300d06092a864886f70d010101050003818b0030818702818100bd040205c15cdde804c64fd54bc8d837c518ac57d3eb"
                            "d0789039746bbfe704f17e2436b19e7dbeeda332f75594202d7c404c08686b7f91832b2a9ec8af7d85c1c49e111a4b669aa8610"
                            "625c92edb4e6f192123d0c9cc962c6b88b677b57d8b89cd86d609ca7d93a9f30015c2e725c5b3ebee720b7f01f94c3c2fc981e8"
                            "341119020111";


    Wallet w(std::move(creds), peers, logger);

    w.pay(pubk_dest, 5);
    
    w.m_creeds->save(creeds_path);

    return 0;
}