#include "core/netmessaging.h"

using namespace networking;

std::string networking::to_string(NetResponse& resp) 
{
    json payload;
    payload["status"] = int(resp.status);
    payload["data"] = resp.data;
    return payload.dump();
}


NetMessage::NetMessage(OP_TYPE opcode, std::string payload) 
{
    op = opcode;
    this->payload = json::parse(payload);
}


std::string networking::to_string(NetMessage& message)
{
    json mes;
    mes["OP"] = int(message.op);
    mes["payload"] = message.payload;
    return mes.dump();
}
