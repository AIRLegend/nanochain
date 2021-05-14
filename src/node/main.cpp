//#include "core/hashing.h"

#include "node.h"
#include "listen_server.h"
#include "nodeconfig.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <iostream>
#include <memory>
#include<unistd.h>

int main()
{
    // Setting up logger
    auto console = spdlog::stdout_color_mt("nanonode");
    spdlog::get("nanonode")->info("Starting up a Nanochain Node");
    std::shared_ptr<spdlog::logger> logger = spdlog::get("nanonode");

    // Load or create config
    // TODO: Pass custom conf path via arguments
    NodeConfig conf;
    try {
        conf = NodeConfig::load("nodeconf.json");
    } catch (std::exception& ex){
        logger->warn("nodeconf.json not found, creating a deafult config");
        conf.save("nodeconf.json");
    }

    // TODO: Hardcoded peers
    NodePeer peer("tcp://127.0.0.1:5556");

    
    Node n(conf, logger);
    std::shared_ptr<IServerSub> node_ptr = std::make_shared<Node>(n);
    
    //n.registerPeer(peer);
    
    ListenServer server(conf, logger);
    server.subscribe(node_ptr);
    server.start();

    while(true)
    {
        n.requestBlocks();
        n.requestTxs();
        sleep(30);
    }

    server.stop();
    

    return 0;
}
