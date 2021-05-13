#ifndef NANOCHAIN_NETCLIENT_H
#define NANOCHAIN_NETCLIENT_H

#include "core/netmessaging.h"
#include "json.hpp"
#include <zmq_addon.hpp>

using json = nlohmann::json;
namespace networking
{

class SenderClient {
public:
    SenderClient();
    ~SenderClient();

    NetResponse send(const std::string data, OP_TYPE msg_type, const std::string address);

private:
    zmq::context_t ctx;
    zmq::socket_t sock_send;
};

}; //namespace networking
#endif //NANOCHAIN_NETCLIENT_H


