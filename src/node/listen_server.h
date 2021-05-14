#ifndef NANOCHAIN_LISTENSERVER_H
#define NANOCHAIN_LISTENSERVER_H


#include <string>
#include <vector>
#include <thread>
#include <memory>
#include <zmq_addon.hpp>

#include "nodeconfig.h"

#include "spdlog/spdlog.h"

#include "core/netmessaging.h"

using json = nlohmann::json;

class IServerSub 
{
public:
    // Requests adding stuff to the server
    virtual void onNewBlock(const json& new_block, networking::NetResponse& response) = 0;
    virtual void onNewTx(const json& new_tx, networking::NetResponse& response) = 0;
    // Requests asking for data
    virtual void onBlockRequest(const json& block_hash, networking::NetResponse& response) = 0;
    virtual void onTxRequest(networking::NetResponse& response) = 0;
};

class ListenServer {
public:

    ListenServer(NodeConfig& conf, std::shared_ptr<spdlog::logger> logger);
    ~ListenServer();

    void start();
    void stop();
    std::string build_msg(const std::string& payload, networking::OP_TYPE op);
    void message_handler(const std::string& message, networking::NetResponse& response);
    void listen();
    void subscribe(std::shared_ptr<IServerSub> sub);


private:
    std::shared_ptr<IServerSub> sub = nullptr;

    std::string listen_addr;

    bool shouldListen = false;

    zmq::context_t ctx;
    zmq::socket_t sock_listen;

    std::unique_ptr<std::thread> listener_thread;

    std::shared_ptr<spdlog::logger> m_logger;
};


#endif //NANOCHAIN_LISTENSERVER_H