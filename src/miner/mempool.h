#ifndef NANOCHAIN_MINER_MEMPOOL_H
#define NANOCHAIN_MINER_MEMPOOL_H

#include "core/transaction.h"
#include "core/block.h"

#include <vector>

class Mempool
{
public:
    Mempool();

    int get(const Transaction& tx);
    bool add(const Transaction& tx);
    bool remove(const Transaction& tx);

    // TODO: This should not be public
    std::vector<Transaction> m_txs;

    bool flush();
private:
    bool compare(const Transaction& t1, const Transaction& t2);
};

#endif //NANOCHAIN_MINER_MEMPOOL_H