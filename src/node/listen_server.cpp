#include "node/listen_server.h"
#include "core/netmessaging.h"

#include "spdlog/spdlog.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/null_sink.h"

#include <iostream>

ListenServer::ListenServer(NodeConfig& conf, 
                           std::shared_ptr<spdlog::logger> logger):
    sock_listen(ctx, zmq::socket_type::rep),
    listen_addr(conf.getListenAddr())
{
    m_logger = logger;
}


ListenServer::~ListenServer() {
}



void ListenServer::listen() {
    m_logger->info("[SERVER] Start listening");

    while(shouldListen) {
        std::vector<zmq::message_t> recv_msgs;
        const auto ret = zmq::recv_multipart(sock_listen, std::back_inserter(recv_msgs));

        if (!ret) {
            shouldListen = false;
            return;
        }

        m_logger->debug("[SERVER] Got a new message!");

        // PROC_MESSAGE
        networking::NetResponse response;
        message_handler(recv_msgs[0].to_string(), response);

        // RESPONSE TO CLIENT
        std::string resp = networking::to_string(response);
        zmq::message_t message(resp.size());
        std::memcpy (message.data(), resp.data(), resp.size());
        sock_listen.send(message, zmq::send_flags::dontwait);
    }
}


void ListenServer::message_handler(const std::string &message, 
                                   networking::NetResponse& response) {
    using namespace networking;
    json json_msg = json::parse(message);
    json payload_json = json_msg["payload"];
    OP_TYPE msg_type = json_msg["OP"];

    switch (msg_type) {
        case OP_BLOCK_REQ:            
            sub->onBlockRequest(payload_json["hash"].get<std::string>(), response);
            break;
        case OP_BLOCK_ANNOUNCE:
            sub->onNewBlock(payload_json, response);
            break;
        case OP_BROADCAST_TX:
            sub->onNewTx(payload_json, response);
            break;
        case OP_MEMPOOL_REQUEST:
            sub->onTxRequest(response);
            break;
        default:
            break;
    }

    // send response
    response.status = MESSAGE_STATUS::OK;
}


void ListenServer::start() {
    sock_listen.bind(listen_addr);
    shouldListen = true;
    listener_thread = std::make_unique<std::thread>(&ListenServer::listen, this);
}

void ListenServer::stop() {
    sock_listen.unbind(listen_addr);
    shouldListen = false;
    //listener_thread->join();
    auto h = listener_thread->native_handle();
    pthread_cancel(h);
    m_logger->info("[SERVER] Stopped listening");
}

void ListenServer::subscribe(std::shared_ptr<IServerSub> sub) {
    this->sub = sub;
    m_logger->info("[SERVER] Registered response handler.");
}


