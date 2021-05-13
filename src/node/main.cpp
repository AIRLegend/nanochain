//#include "core/hashing.h"

#include "node.h"
#include "listen_server.h"

#include <iostream>

#include <memory>
#include<unistd.h>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

int main()
{
    // Setting up logger
    auto console = spdlog::stdout_color_mt("console");
    spdlog::get("console")->info("Starting up a Nanochain Node");

    // TODO: Hardcoded peers
    NodePeer peer;
    peer.address = "tcp://127.0.0.1:5556";

    
    Node n;
    std::shared_ptr<IServerSub> node_ptr = std::make_shared<Node>(n);

    //n.m_peers.push_back(peer);
    
    ListenServer server("tcp://0.0.0.0:5557");
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
