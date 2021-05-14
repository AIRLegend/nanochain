#ifndef NANOCHAIN_NODECONF_H
#define NANOCHAIN_NODECONF_H

#include <string>

class NodeConfig
{
public:
    
    unsigned int getPoolingTime();
    unsigned int getListenPort();
    std::string getListenAddr();

    static NodeConfig load(const std::string& path);
    void save(const std::string& path);

private:
    unsigned int polling_time  = 30;
    unsigned int listen_port   = 5557;
    std::string listen_addr    = "tcp://0.0.0.0";
};

#endif  //NANOCHAIN_NODECONF_H