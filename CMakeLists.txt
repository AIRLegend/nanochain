cmake_minimum_required(VERSION 3.17)
project(nanochain)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


include_directories(/usr/local/Cellar/zeromq/4.3.4/include/)
include_directories(vendor/cryptopp/
                    vendor/json/
                    vendor/zeromq/
                    src)


# Build Cryptopp
add_subdirectory(vendor/cryptopp)
find_library(vendor/cryptopp libcryptopp)
set_target_properties(cryptopp-static PROPERTIES EXCLUDE_FROM_ALL True)
set_target_properties(cryptopp-static  PROPERTIES CXX_EXTENSIONS OFF)

# Common library for all network components
add_library(nanochain_core
            src/core/hashing.cpp
            src/core/sign.cpp
            )
target_link_libraries(nanochain_core cryptopp-static)
set_target_properties(nanochain_core  PROPERTIES CXX_EXTENSIONS OFF)


# Node program
add_executable(node src/core/node/main.cpp)
target_link_libraries(node nanochain_core)
set_target_properties(node  PROPERTIES CXX_EXTENSIONS OFF)

