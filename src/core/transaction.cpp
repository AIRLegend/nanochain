#include "core/transaction.h"
#include "core/sign.h"

Transaction::Transaction(
                unsigned char* from,
                unsigned char* to,
                uint64_t amount)
{
    if (from == nullptr)
        memset(this->m_from, 0, SIGNATURE_LEN);
    else
        memcpy(this->m_from, from, SIGNATURE_LEN);

    memcpy(this->m_to, to, SIGNATURE_LEN);

    // The transaction must be manually signed
    memset(m_signature, 0, SIGNATURE_LEN);
    m_amount = amount;
}

Transaction::Transaction(const Transaction& tx) 
{
    memcpy(this->m_from, tx.m_from, SIGNATURE_LEN);
    memcpy(this->m_to, tx.m_to, SIGNATURE_LEN);
    memcpy(m_signature, tx.m_signature, SIGNATURE_LEN);
    m_amount = tx.m_amount;
}