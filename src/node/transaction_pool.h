#ifndef NANOCHAIN_TXPOOL_H
#define NANOCHAIN_TXPOOL_H

#include <vector>

#include "core/transaction.h"

/**
 * It is, basically, the mempool.
 * It holds all the transactions which have not been included in blocks yet.
 * The miner will grab them from here and include in their produced blocks.
 */
class TransactionPool {
public:
    TransactionPool();
    int get(const Transaction& tx);
    bool add(const Transaction& tx);
    bool remove(const Transaction& tx);
    std::vector<Transaction>& getTxs();

private:
    std::vector<Transaction> m_txs;
    bool compare(const Transaction& t1, const Transaction& t2);
};

#endif //NANOCHAIN_TXPOOL_H