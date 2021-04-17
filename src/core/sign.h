#ifndef NANOCHAIN_SIGN_H
#define NANOCHAIN_SIGN_H

#include <dll.h>
#include <rsa.h>

static const int SIGNATURE_LEN = 1024;
static const int PREIMAGE_LEN = 128;

class PubKey {
public:
    unsigned char PUB[SIGNATURE_LEN];

    PubKey(CryptoPP::InvertibleRSAFunction &params);
    PubKey(unsigned char *key, size_t keysize = SIGNATURE_LEN);
    PubKey(PubKey &ref);

    bool validate(unsigned char *data, int datalen, unsigned char *signature);

private:
    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::InvertibleRSAFunction param;
    CryptoPP::RSA::PublicKey publicKey;
};

class PrivKey {
public:
    PrivKey(CryptoPP::InvertibleRSAFunction &params);
    PrivKey(unsigned char *key, size_t keysize = SIGNATURE_LEN);
    PrivKey(PrivKey &ref);

    unsigned char *sign(unsigned char *out, unsigned char *data, int datalen = 0);

private:
    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::InvertibleRSAFunction params;
    CryptoPP::RSA::PrivateKey privateKey;
};

struct KeyPair {
    std::shared_ptr<PubKey> publicKey;
    std::shared_ptr<PrivKey> privateKey;
};

class KeyFactory {
public:
    static std::unique_ptr<KeyPair> buildKeyPair();
};

#endif
