//#include "core/hashing.h"

#include "node.h"
#include "listen_server.h"

#include <iostream>

int main()
{
    /*unsigned  char* hashout2 = new unsigned  char[HASH_SIZE];
    std::string message = "abcdefghijklmnopqrstuvwxyz";
    sha256((void *)message.data(), hashout2, message.length());
    std::cout << "HASH: " << sha2str(hashout2) << std::endl;*/

    ListenServer server;
    std::shared_ptr<Node> n = std::make_shared<Node>();

    server.subscribe(n);


    return 0;
}
