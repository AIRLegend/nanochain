#ifndef NANOCHAIN_SIGN_H
#define NANOCHAIN_SIGN_H

#include <dll.h>
#include <rsa.h>

static const int SIGNATURE_LEN = 1024;
static const int PREIMAGE_LEN = 128;

class PubKey {
public:
    static const size_t PUBK_LEN = 160;
    unsigned char m_bytes[PUBK_LEN];
    

    PubKey(CryptoPP::InvertibleRSAFunction &params);
    PubKey(unsigned char *key, size_t keysize = PUBK_LEN);
    PubKey(PubKey &ref);

    static size_t getLength();

    bool validate(unsigned char *data, int datalen, unsigned char *signature);

private:
    CryptoPP::AutoSeededRandomPool m_rng;
    CryptoPP::InvertibleRSAFunction m_params;
    CryptoPP::RSA::PublicKey m_publicKey;
};

class PrivKey {
public:
    static const size_t PK_LEN = 632;

    unsigned char m_bytes[PrivKey::PK_LEN];

    PrivKey(CryptoPP::InvertibleRSAFunction &params);
    PrivKey(unsigned char *key, size_t keysize = PrivKey::PK_LEN);
    PrivKey(PrivKey &ref);

    unsigned char *sign(unsigned char *out, unsigned char *data, int datalen = 0);

private:
    CryptoPP::AutoSeededRandomPool m_rng;
    CryptoPP::InvertibleRSAFunction m_params;
    CryptoPP::RSA::PrivateKey m_privateKey;
};

struct KeyPair {
    std::shared_ptr<PubKey> publicKey;
    std::shared_ptr<PrivKey> privateKey;
};

class KeyFactory {
public:
    static std::shared_ptr<KeyPair> buildKeyPair();
    static std::shared_ptr<KeyPair> buildKeyPair( unsigned char* pk, 
                                                  unsigned char* pubk);
};

#endif
