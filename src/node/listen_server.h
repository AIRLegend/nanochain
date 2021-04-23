#ifndef NANOCHAIN_LISTENSERVER_H
#define NANOCHAIN_LISTENSERVER_H


#include <string>
#include <vector>
#include <thread>
#include <memory>
#include <zmq_addon.hpp>

#include "core/netmessaging.h"

class IServerSub 
{
public:
    virtual json onNewBlock(const std::string new_block) = 0;
    virtual json onBlockRequest(const std::string blockHash) = 0;
};

class ListenServer {
public:

    ListenServer(std::string listen_address="tcp://0.0.0.0:5556");
    ~ListenServer();

    void start();
    void stop();
    std::string build_msg(const std::string& payload, networking::OP_TYPE op);
    void message_handler(const std::string& message);
    void listen();
    void subscribe(std::shared_ptr<IServerSub> sub);


private:
    std::shared_ptr<IServerSub> sub;

    std::string lister_addr;

    bool shouldListen = false;

    zmq::context_t ctx;
    zmq::socket_t sock_listen;

    std::unique_ptr<std::thread> listener_thread;
};


#endif //NANOCHAIN_LISTENSERVER_H