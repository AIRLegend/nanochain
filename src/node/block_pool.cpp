#include "node/block_pool.h"

#include "core/block.h"
#include "core/transaction.h"

BlockPool::BlockPool() {
    // Generate addresses
    auto keypair_u1 = KeyFactory::buildKeyPair();
    auto keypair_u2 = KeyFactory::buildKeyPair();
    auto keypair_u3 = KeyFactory::buildKeyPair();

    // Simulate a blockchain
    Block genesis;
    memset(genesis.b_hash, 0, HASH_SIZE);
    memset(genesis.prev_hash, 0, HASH_SIZE);

    Transaction coinbase((unsigned char*)nullptr, keypair_u1->publicKey->m_bytes, 10);  // Unsigned, genesis


    // u1 pays u2 5 and signs
    Transaction t1(keypair_u1->publicKey->m_bytes, keypair_u2->publicKey->m_bytes, 5);
    keypair_u1->privateKey->sign(t1.m_signature, (unsigned char*)&t1, sizeof(t1));

    // u1 pays u3 10
    Transaction t2(keypair_u1->publicKey->m_bytes, keypair_u3->publicKey->m_bytes, 10);
    keypair_u1->privateKey->sign(t2.m_signature, (unsigned char*)&t2, sizeof(t2));

    // Add transactions to block
    genesis.txs.push_back(coinbase);
    genesis.txs.push_back(t1);
    genesis.txs.push_back(t2);

    sha256(&genesis, genesis.b_hash, sizeof(genesis));

    blocks.push_back(genesis);

    // Ahora generamos block 2
    Block block_1;
    memset(block_1.b_hash, 0, HASH_SIZE);

    Transaction t3(keypair_u1->publicKey->m_bytes, keypair_u2->publicKey->m_bytes, 8);
    keypair_u1->privateKey->sign(t3.m_signature, (unsigned char*)&t3, sizeof(t3));

    Transaction t4(keypair_u3->publicKey->m_bytes, keypair_u3->publicKey->m_bytes, 1);
    keypair_u3->privateKey->sign(t4.m_signature, (unsigned char*)&t4, sizeof(t4));


    block_1.txs.push_back(coinbase);
    block_1.txs.push_back(t3);
    block_1.txs.push_back(t4);


    sha256(&block_1, block_1.b_hash, sizeof(block_1));
    memcpy(block_1.prev_hash, genesis.b_hash, HASH_SIZE);

    blocks.push_back(std::move(block_1));
}
