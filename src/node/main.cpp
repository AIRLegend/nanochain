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

    /*unsigned  char* hashout2 = new unsigned  char[HASH_SIZE];
    std::string message = "abcdefghijklmnopqrstuvwxyz";
    sha256((void *)message.data(), hashout2, message.length());
    std::cout << "HASH: " << sha2str(hashout2) << std::endl;*/

    ListenServer server("tcp://0.0.0.0:5556");
    std::shared_ptr<IServerSub> n = std::make_shared<Node>();

    server.subscribe(n);

    server.start();

    sleep(5);

    server.stop();

    return 0;
}
