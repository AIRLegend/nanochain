#include "core/netmessaging.h"

std::string networking::to_string(NetResponse& resp) 
{
    json payload;
    payload["status"] = std::to_string(resp.status);
    payload["data"] = resp.data;
    return payload.dump();
}



