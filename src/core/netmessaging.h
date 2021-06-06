#ifndef NANOCHAIN_NETMESSAGING_H
#define NANOCHAIN_NETMESSAGING_H

#include "json.hpp"

using json = nlohmann::json;

/**
 *  Definitions for handling node message serialization/deserialization.
 * 
 *  Also contains definitions of all commands supported by the network
 */ 
namespace networking
{

/**
 * Possible requests/messages types
 */
enum OP_TYPE {
    OP_NULL = 0,                    // NULL operation. Does nothing
    OP_BROADCAST_TX = 1,            // For indicating a message is a transaction announcement
    OP_BLOCK_SEND = 3,              // For indicating a message is a request for a certain block
    OP_BLOCK_REQ = 4,               // For indicating a message is a request of a certain block
    OP_BLOCK_ANNOUNCE = 5,          // For indicating a message is a block announcement (usually sent by miners)
    OP_MEMPOOL_REQUEST = 6,         // For indicating a message is a request for all the unprocessed transactions a node has (usually sent by miners)
    OP_BALANCE_REQUEST = 7          // For requesting the current balance of a certain address.
};

/**
 * Possible response status
 */
enum MESSAGE_STATUS {
    BAD = 0,  // Something failed
    OK = 1    // No problems
};

/**
 * Definition of network responses. They have a STATUS and a payload
 */
struct NetResponse {
    MESSAGE_STATUS status;
    json data;
};

/**
 * Definition of message operations. They all have a OP_TYPE identifying the request type the server must
 * process and a payload with additional data (different for each OP_TYPE)
 */
struct NetMessage {
    OP_TYPE op;
    json payload;

    NetMessage(OP_TYPE opcode, std::string payload);
};

// Serialization / Deserialization for sending messages over the network.
std::string to_string(NetResponse& resp);
std::string to_string(NetMessage& message);

}; //namespace networking
#endif //NANOCHAIN_NETMESSAGING_H


