#ifndef NANOCHAIN_NODEPEER_H
#define NANOCHAIN_NODEPEER_H

#include <string>

class NodePeer 
{
public:
    std::string address;
private:
    // TODO: used for tracking trustworthiness of the peer 
    float reputation;
};

#endif //NANOCHAIN_NODEPEER_H