#ifndef NANOCHAIN_TRANSACTION_H
#define NANOCHAIN_TRANSACTION_H

#include <core/sign.h>

class Transaction
{
public:

    static const int SIGLEN = SIGNATURE_LEN;

    Transaction(unsigned char* from,
                unsigned char* to,
                uint64_t amount=0);

    Transaction(const Transaction& tx);

    unsigned char m_signature[SIGNATURE_LEN];
    unsigned char m_from[SIGNATURE_LEN];
    unsigned char m_to[SIGNATURE_LEN];

    int getAmount() const { return m_amount; };
private: 
    int m_amount;    
};

#endif //NANOCHAIN_TRANSACTION_H
