#ifndef NANOCHAIN_NETMESSAGING_H
#define NANOCHAIN_NETMESSAGING_H

#include "json.hpp"

using json = nlohmann::json;
namespace networking
{
    
enum OP_TYPE {
    OP_NULL = 0,
    OP_BROADCAST_TX = 1,
    OP_BLOCK_SEND = 3,
    OP_BLOCK_REQ = 4,
    OP_BLOCK_ANNOUNCE = 5,
    OP_MEMPOOL_REQUEST = 6
};

enum MESSAGE_STATUS {
    BAD = 0,
    OK = 1
};

struct NetResponse {
    MESSAGE_STATUS status;
    json data;
};

std::string to_string(NetResponse& resp);

}; //namespace networking
#endif //NANOCHAIN_NETMESSAGING_H


