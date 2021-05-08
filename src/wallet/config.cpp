#include "wallet/config.h"
#include "core/serializing.h"

std::unique_ptr<Creedentials> Creedentials::fromJson(json& creedentials) {
    std::unique_ptr<Creedentials> cred = std::make_unique<Creedentials>();

    unsigned char restored_pk[PrivKey::PK_LEN];
    unsigned char restored_pubk[PubKey::PUBK_LEN];

    std::string priv_serialized = creedentials["pk"];
    std::string pub_serialized = creedentials["pubk"];

    stringToBytes(priv_serialized, restored_pk);
    stringToBytes(pub_serialized, restored_pubk);
    
    cred->m_keypair = KeyFactory::buildKeyPair(restored_pk, restored_pubk);
    return std::move(cred);
}
std::unique_ptr<Creedentials> Creedentials::fromFile(std::string& file_path) 
{  
    std::unique_ptr<Creedentials> creds;

    try {
        json creed_json = fromDisk(file_path);
        creds =  Creedentials::fromJson(creed_json);
    } catch (json::exception& ex) {
        std::cerr << ex.what() << std::endl;
        creds = Creedentials::create();
    }

    return std::move(creds);
}

std::unique_ptr<Creedentials> Creedentials::create() 
{
    std::unique_ptr<Creedentials> cred = std::make_unique<Creedentials>();
    cred->m_keypair = KeyFactory::buildKeyPair();  
    return std::move(cred);
}


json Creedentials::toJson() 
{
    std::string pk_str = bytesToString(m_keypair->privateKey->m_bytes, m_keypair->privateKey->PK_LEN);
    std::string pubk_str = bytesToString(m_keypair->publicKey->m_bytes, m_keypair->publicKey->PUBK_LEN);

    json repr = {
            {"pk", pk_str},
            {"pubk", pubk_str},
    }; 

    return repr;
}

bool Creedentials::save(std::string& path) 
{
    json json_kp = toJson();
    toDisk(path, json_kp);
    return true;
}
