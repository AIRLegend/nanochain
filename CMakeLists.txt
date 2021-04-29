cmake_minimum_required(VERSION 3.17)
project(nanochain)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

### ZEROMQ
## use the hint from about to find the location of libzmq
## load in pkg-config support
find_package(PkgConfig)
## use pkg-config to get hints for 0mq locations
pkg_check_modules(PC_ZeroMQ QUIET zmq)
find_library(ZeroMQ_LIBRARY
        NAMES zmq
        PATHS ${PC_ZeroMQ_LIBRARY_DIRS}
        )
#######

# Logging
add_subdirectory(vendor/spdlog)
find_package(spdlog)


include_directories(/usr/local/Cellar/zeromq/4.3.4/include/)
include_directories(vendor/cryptopp/
                    vendor/json/
                    vendor/cppzmq/
                    src)


# Build Cryptopp
add_subdirectory(vendor/cryptopp)
find_library(vendor/cryptopp libcryptopp)
set_target_properties(cryptopp-static PROPERTIES EXCLUDE_FROM_ALL True)

# Common library for all core and networking components
add_library(nanochain_core
            src/core/hashing.cpp
            src/core/sign.cpp
            src/core/transaction.cpp
            src/core/block.cpp
            src/core/serializing.cpp
            src/core/netmessaging.cpp
            src/core/netclient.cpp
            )
target_link_libraries(nanochain_core cryptopp-static)


# Node program
add_executable(node 
    src/node/main.cpp
    src/node/node.cpp
    src/node/block_pool.cpp
    src/node/listen_server.cpp
    src/node/transaction_pool.cpp
    )

target_link_libraries(node nanochain_core)
target_link_libraries(node ${ZeroMQ_LIBRARY})
target_link_libraries(node spdlog)