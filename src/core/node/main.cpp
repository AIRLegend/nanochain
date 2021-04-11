#include <core/hashing.h>
#include <iostream>

int main()
{

    unsigned  char* hashout2 = new unsigned  char[HASH_SIZE];
    std::string message = "abcdefghijklmnopqrstuvwxyz";
    sha256(message.data(), hashout2, message.length());
    std::cout << "HASH: " << sha2str(hashout2) << std::endl;
    return 0;
}
