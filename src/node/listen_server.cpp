#include "node/listen_server.h"
#include "core/netmessaging.h"

#include "spdlog/spdlog.h"

#include <iostream>

ListenServer::ListenServer(std::string listen_address):
    sock_listen(ctx, zmq::socket_type::rep),
    listen_addr(listen_address)
{

}


ListenServer::~ListenServer() {
}



void ListenServer::listen() {
    spdlog::get("console")->info("[SERVER] Start listening");

    while(shouldListen) {
        std::vector<zmq::message_t> recv_msgs;
        const auto ret = zmq::recv_multipart(sock_listen, std::back_inserter(recv_msgs));
        if (!ret) {
            shouldListen = false;
            return;
        }

        //std::cout << "Got " << *ret << " messages" << std::endl;
        //std::cout << "\t" << recv_msgs[0].to_string()<< std::endl;

        // PROC_MESSAGE
        networking::NetResponse response;
        message_handler(recv_msgs[0].to_string(), response);

        // RESPONSE TO CLIENT
        std::string resp = networking::to_string(response); //= recv_msgs[0].to_string();
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
            sub->onBlockRequest(payload_json["hash"], response);
            break;
        case OP_BLOCK_ANNOUNCE:
            sub->onNewBlock(payload_json.dump(), response);
            break;
        default:
            break;
    }

    // send response
    //
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
    spdlog::get("console")->info("[SERVER] Stopped listening");
}

void ListenServer::subscribe(std::shared_ptr<IServerSub> sub) {
    this->sub = sub;
    spdlog::get("console")->info("[SERVER] Registered response handler.");
}


