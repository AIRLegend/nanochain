#ifndef NANOCHAIN_BLOCKPOOL_H
#define NANOCHAIN_BLOCKPOOL_H

#include <vector>
#include "core/block.h"

class BlockPool {
public:
    BlockPool();

    std::vector<Block> blocks;

    // Dummy for easing development
    Block generateBlock();

private:

};


#endif //NANOCHAIN_BLOCKPOOL_H