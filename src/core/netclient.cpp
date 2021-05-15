#include "core/netclient.h"

#include <iostream>

using namespace networking;

SenderClient::SenderClient():
    sock_send(ctx, zmq::socket_type::req)
{
    sock_send.set( zmq::sockopt::req_relaxed, 1 );
}

SenderClient::~SenderClient() {
    sock_send.close();
}

NetResponse SenderClient::send(const std::string data, OP_TYPE msg_type, const std::string address) {
    NetResponse net_response;
    NetMessage net_mess(msg_type, data);

    sock_send.connect(address);

    std::string msg = to_string(net_mess);
    zmq::message_t message(msg.size());
    std::memcpy (message.data(), msg.data(), msg.size());

    sock_send.send(message, zmq::send_flags::dontwait);

    std::vector<zmq::message_t> recv_msgs;
    zmq::message_t mes;
    std::string mes_str = "";
    for (int attempts=0; attempts < 2; attempts++) {
        //const auto ret = zmq::recv_multipart(sock_send, std::back_inserter(recv_msgs), zmq::recv_flags::dontwait);
        sock_send.recv(mes, zmq::recv_flags::dontwait);
        mes_str = mes.to_string();

        if (mes_str.size() > 0) {
            break;
        }
        // TODO: configurable timeout
        zmq_sleep(2);
    }

    sock_send.disconnect(address);


    if (mes_str.size() <= 0)
        net_response.status = MESSAGE_STATUS::BAD;
    else
    {
        //std::string response = recv_msgs[0].to_string();
        json resp_json = json::parse(mes_str);
        net_response.status = resp_json["status"];
        net_response.data = resp_json["data"];
    }

    return net_response;
}