//#include "core/hashing.h"

#include "node.h"
#include "listen_server.h"

#include <iostream>

#include <memory>
#include<unistd.h>

int main()
{
    // Setting up logger
    auto console = spdlog::stdout_color_mt("nanonode");
    spdlog::get("nanonode")->info("Starting up a Nanochain Node");
    std::shared_ptr<spdlog::logger> logger = spdlog::get("nanonode");


    // TODO: Hardcoded peers
    NodePeer peer;
    peer.address = "tcp://127.0.0.1:5556";

    
    Node n;
    std::shared_ptr<IServerSub> node_ptr = std::make_shared<Node>(n);
    
    //n.registerPeer(peer);
    
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
