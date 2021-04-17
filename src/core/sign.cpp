#include "sign.h"

#include <memory>

// PRIVATE KEY
PrivKey::PrivKey(PrivKey &ref) :
        m_privateKey(ref.m_params),
        m_params(ref.m_params)
{
    //this->m_rng = ref.m_rng;
}

PrivKey::PrivKey(CryptoPP::InvertibleRSAFunction &params):
    m_privateKey(params),
    m_params(params)
{
    //params.GenerateRandomWithKeySize(*this->m_rng, 1024);

}

PrivKey::PrivKey(unsigned char* key, size_t keysize)
{
    CryptoPP::ArraySource arsource(key, keysize, true);
    m_privateKey.Load(arsource);
}

unsigned char * PrivKey::sign(unsigned char *out, unsigned char *data, int datalen) {
    CryptoPP::RSASSA_PKCS1v15_SHA_Signer privkeySigner(m_privateKey);
    //unsigned char *signature = new unsigned char[SIGNATURE_LEN];
    privkeySigner.SignMessage(m_rng, data, datalen, out);
    return out;
}


// PUBLIC KEY

PubKey::PubKey(PubKey &ref):
    m_publicKey(ref.m_params),
    m_params()
{
    ref.m_params.GetThisObject(m_params);
}

PubKey::PubKey(CryptoPP::InvertibleRSAFunction &params):
    m_publicKey(params),
    m_params(params)
{
    // Set internal PK representation
    CryptoPP::ArraySink buf_pub = CryptoPP::ArraySink(PUB, 1024);
    m_publicKey.DEREncode(buf_pub);
    buf_pub.MessageEnd();
}

PubKey::PubKey(unsigned char* key, size_t keysize):
    m_publicKey()
{
    CryptoPP::ArraySource arsource(key, keysize, true);
    m_publicKey.Load(arsource);
}

bool PubKey::validate(unsigned char *data, int datalen, unsigned char *signature) {
    CryptoPP::RSASSA_PKCS1v15_SHA_Verifier verifierPub(m_publicKey);
    bool valid = verifierPub.VerifyMessage(data,
                                           datalen,
                                           signature,
                                           PREIMAGE_LEN);
    return valid;
}


// KeyFactory
std::unique_ptr<KeyPair>  KeyFactory::buildKeyPair() {
    std::unique_ptr<CryptoPP::InvertibleRSAFunction> custom_params =
            std::make_unique<CryptoPP::InvertibleRSAFunction>();
    CryptoPP::AutoSeededRandomPool randg;
    custom_params->GenerateRandomWithKeySize(randg, 1024);

    std::unique_ptr<KeyPair> kp = std::make_unique<KeyPair>();
    kp->publicKey = std::make_shared<PubKey>(*custom_params);
    kp->privateKey = std::make_shared<PrivKey>(*custom_params);

    return std::move(kp);
}