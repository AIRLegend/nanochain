#include "nodeconfig.h"

#include "core/serializing.h"

#include "json.hpp"

using json = nlohmann::json;

NodeConfig NodeConfig::load(const std::string& path) 
{
    NodeConfig conf;

    json jsonconf = fromDisk(path);

    conf.listen_port = jsonconf["server"]["listen_port"];
    conf.polling_time = jsonconf["node"]["polling_time"];

    return conf;
}

void NodeConfig::save(const std::string& path) 
{
    json server_conf = {
        {"listen_port", listen_port}
    };

    json node_conf = {
        {"polling_time", polling_time}
    };

    json root = {
        {"server", server_conf},
        {"node", node_conf}
    };

    toDisk(path, root);
}



/*
    GETTERS
*/
unsigned int NodeConfig::getPoolingTime() 
{
    return polling_time;
}

unsigned int NodeConfig::getListenPort() 
{
    return listen_port;
}

std::string NodeConfig::getListenAddr() 
{
    return listen_addr;
}
