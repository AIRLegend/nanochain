#include "miner/miner.h"
#include "core/peer.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <unistd.h>

int main() {
    auto console = spdlog::stdout_color_mt("nanominer");
    spdlog::get("nanominer")->info("Starting up a Nanochain Miner");

    Miner miner;

    // TODO: Hardcoded peers
    NodePeer peer;
    peer.address = "tcp://127.0.0.1:5556";
    miner.addPeer(peer);
    
    while(true) {
        miner.requestMempool();
        miner.startMining();
        sleep(10); // TODO: Variable pooling rate
    }
    
    miner.stopMining();

    return 0;
}