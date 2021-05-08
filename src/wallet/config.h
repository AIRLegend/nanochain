#ifndef NANOCHAIN_WALLET_CONF_H
#define NANOCHAIN_WALLET_CONF_H

#include "json.hpp"
#include "core/sign.h"
#include "core/serializing.h"
#include <memory>


using json = nlohmann::json;

class Creedentials {
public:
    std::shared_ptr<KeyPair> m_keypair;

    json toJson();
    bool save(std::string& path);

    static std::unique_ptr<Creedentials> fromJson(json& creedentials);
    static std::unique_ptr<Creedentials> fromFile(std::string& file_path);
    static std::unique_ptr<Creedentials> create();
};

//class Config {
//    static Config from_json(json creedentials);
//};

#endif //NANOCHAIN_WALLET_CONF_H