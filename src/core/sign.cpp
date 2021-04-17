#include "sign.h"

#include <memory>

// PRIVATE KEY
PrivKey::PrivKey(PrivKey &ref) :
        privateKey(ref.params),
        params(ref.params)
{
    //this->rng = ref.rng;
}

PrivKey::PrivKey(CryptoPP::InvertibleRSAFunction &params):
    privateKey(params),
    params(params)
{
    //params.GenerateRandomWithKeySize(*this->rng, 1024);

}

PrivKey::PrivKey(unsigned char* key, size_t keysize)
{
    CryptoPP::ArraySource arsource(key, keysize, true);
    privateKey.Load(arsource);
}

unsigned char * PrivKey::sign(unsigned char *out, unsigned char *data, int datalen) {
    CryptoPP::RSASSA_PKCS1v15_SHA_Signer privkeySigner(privateKey);
    //unsigned char *signature = new unsigned char[SIGNATURE_LEN];
    privkeySigner.SignMessage(rng, data, datalen, out);
    return out;
}


// PUBLIC KEY

PubKey::PubKey(PubKey &ref):
    publicKey(ref.param),
    param()
{
    ref.param.GetThisObject(param);
}

PubKey::PubKey(CryptoPP::InvertibleRSAFunction &params):
    publicKey(params),
    param(params)
{
    // Set internal PK representation
    CryptoPP::ArraySink buf_pub = CryptoPP::ArraySink(PUB, 1024);
    publicKey.DEREncode(buf_pub);
    buf_pub.MessageEnd();
}

PubKey::PubKey(unsigned char* key, size_t keysize):
    publicKey()
{
    CryptoPP::ArraySource arsource(key, keysize, true);
    publicKey.Load(arsource);
}

bool PubKey::validate(unsigned char *data, int datalen, unsigned char *signature) {
    CryptoPP::RSASSA_PKCS1v15_SHA_Verifier verifierPub(publicKey);
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