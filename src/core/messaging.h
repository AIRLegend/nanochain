#ifndef NANOCHAIN_MESSAGING_H
#define NANOCHAIN_MESSAGING_H

#include "json.hpp"

using json = nlohmann::json;
namespace networking
{
    
enum OP_TYPE {
    OP_NULL = 0,
    OP_BROADCAST_TX = 1,
    OP_BLOCK_SEND = 3,
    OP_BLOCK_REQ = 4,
    OP_BLOCK_ANNOUNCE = 5
};

enum MESSAGE_STATUS {
    BAD = 0,
    OK = 1
};

struct NetResponse {
    MESSAGE_STATUS status;
    json data;
};

}; //namespace networking
#endif //NANOCHAIN_MESSAGING_H


