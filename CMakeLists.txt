cmake_minimum_required(VERSION 3.17)
project(nanochain)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(vendor/zeromq/)
include_directories(/usr/local/Cellar/zeromq/4.3.4/include/)
include_directories(vendor/json/)
include_directories(vendor/cryptopp/)

add_subdirectory(vendor/cryptopp)
find_library(vendor/cryptopp libcryptopp)

add_library(nanochain_core src/core/hashing.cpp src/core/hashing.h)

