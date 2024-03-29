# nanochain
This project holds a simplified implementation of a blockchain network.

The only purpose of this implementation is to better understand the foundations of a generic PoW-based network.


## Dependencies

- **ZeroMQ**
    - On MacOS: `brew install zeromq`
    - On Linux: Visit https://zeromq.org/download/

## Building

``` bash
git clone git@github.com:AIRLegend/nanochain.git && cd nanochain
mkdir build && cd build
cmake ..
make
```

## Running
```
# Node program
./build/bin/node

# Miner program
./build/bin/miner

# Wallet program
./build/bin/wallet
```


## Project structure at a high level

It's divided into several targets:

### core
Static library containing common network functionalities (definition and implementation of components like serialization, blocks, signing, hashing or transactions). All programs made for this network may link this target.

### node
Executable for running a node of the network, incharged of receiving and broadcasting transactions and blocks, maintaining a local copy of the blockchain and validating it.

### miner
Program which queries the nodes for new transactions and creates (and broadcasts) new blocks using the PoW approximation.

### wallet
Little utility for creating and broadcasting transactions.


## Playing with it
After building, simply start a node with 
```
./build/bin/node
````

It will start listening for transactions. Then, you can send a dummy TX running the wallet program.

```
./build/bin/wallet
```

The transaction will be registered on the node's mempool. Then, run the miner for retreiving it and creating a new block. The block will also be broadcasted to the node.

```
./build/bin/miner
```