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

    NodePeer peer("127.0.0.1:5556");
    std::vector peers = { peer };

    std::string creeds_path = "creeds.json";
    std::unique_ptr<Creedentials> creds = Creedentials::fromFile(creeds_path);

    Wallet w(std::move(creds));

    w.m_creeds->save(creeds_path);

    return 0;
}