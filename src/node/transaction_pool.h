#ifndef NANOCHAIN_TXPOOL_H
#define NANOCHAIN_TXPOOL_H

#include <vector>

#include "core/transaction.h"

class TransactionPool {
public:
    int get(const Transaction& tx);
    bool add(const Transaction& tx);
    bool remove(const Transaction& tx);

private:
    std::vector<Transaction> txs;
    bool compare(const Transaction& t1, const Transaction& t2);
};

#endif //NANOCHAIN_TXPOOL_H