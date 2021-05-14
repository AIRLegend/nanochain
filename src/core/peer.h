#ifndef NANOCHAIN_NODEPEER_H
#define NANOCHAIN_NODEPEER_H

#include <string>

class NodePeer 
{
public:
    NodePeer(const std::string& address, float reputation=10) {
        this->address = address;
    }

    std::string address;
    float reputation;
private:
    // TODO: used for tracking trustworthiness of the peer 
    
};

#endif //NANOCHAIN_NODEPEER_H