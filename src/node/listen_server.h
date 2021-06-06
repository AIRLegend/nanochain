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

/**
 * Interface for callbacks of the Node server process 
 */
class IServerSub 
{
public:
    // Requests adding stuff to the server
    virtual void onNewBlock(const json& new_block, networking::NetResponse& response) = 0;
    virtual void onNewTx(const json& new_tx, networking::NetResponse& response) = 0;

    // Requests asking for data
    virtual void onBlockRequest(const json& block_hash, networking::NetResponse& response) = 0;
    virtual void onBalanceRequest(const json& address, networking::NetResponse& response) = 0;
    virtual void onTxRequest(networking::NetResponse& response) = 0;
};


/**
 * Each node (network listener) will have a server listening for request of other nodes. This 
 * class handles listening/networking functionality and will notify all the subscribed objects 
 * with the received messages. All the subscribed objects must implement IServerSub. 
 */
class ListenServer {
public:

    ListenServer(NodeConfig& conf, std::shared_ptr<spdlog::logger> logger);
    ~ListenServer();

    /**
     * Starts listening 
     */
    void start();

    /**
     * Free resources and stops listening 
     */
    void stop();

    /**
     * This is where the message serialization/deserialization happens. Depending on the 
     * request OP_TYPE, each callback will be executed.
     */ 
    void message_handler(const std::string& message, networking::NetResponse& response);

    
    /**
     * Adds an objects to the subscriber list so it will be notified on each new event.
     */
    void subscribe(std::shared_ptr<IServerSub> sub);


private:
    std::shared_ptr<IServerSub> sub = nullptr;

    std::string listen_addr;

    bool shouldListen = false;

    zmq::context_t ctx;
    zmq::socket_t sock_listen;

    std::unique_ptr<std::thread> listener_thread;

    std::shared_ptr<spdlog::logger> m_logger;

    /**
     * Main listening loop
     */
    void listen();
};


#endif //NANOCHAIN_LISTENSERVER_H