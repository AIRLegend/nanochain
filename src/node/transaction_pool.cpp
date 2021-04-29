#include "node/transaction_pool.h"
#include "core/hashing.h"


int TransactionPool::get(const Transaction& tx) {
    for (int i = 0; i < m_txs.size(); ++i) {
        if (compare(tx, m_txs.at(i))) {
            return i;
        }
    }
    return -1;
}

bool TransactionPool::add(const Transaction& tx) {
    int idx_tx = get(tx);
    if (idx_tx >= 0)
        return false;

    m_txs.push_back(tx);
    return true;
}

bool TransactionPool::remove(const Transaction& tx) {
    int idx_tx = get(tx);
    if (idx_tx < 0)
        return false;
    m_txs.erase(m_txs.begin() + idx_tx);
    return true;
}

std::vector<Transaction>& TransactionPool::getTxs() 
{
    return m_txs;
}

bool TransactionPool::compare(const Transaction& t1, const Transaction& t2) {

    unsigned char hash1[HASH_SIZE];
    unsigned char hash2[HASH_SIZE];
    sha256((void *) &t1, hash1, sizeof(t1));
    sha256((void *) &t2, hash2, sizeof(t2));

    bool eq = memcmp(hash1, hash2, HASH_SIZE) == 0;
    return eq;
}