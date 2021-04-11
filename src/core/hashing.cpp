#include <ios>
#include <sstream>
#include <iomanip>
#include "hashing.h"


void sha256(void* in, unsigned char* out, size_t len){
    CryptoPP::SHA256 h;
    h.CalculateDigest(out, (unsigned  char*)in, len);
}

/*TODO: Improve this, but it works for the moment.*/
std::string sha2str(const unsigned char *hash) {
    std::stringstream hashStr;
    for (int i=0; i<HASH_SIZE; i++) {
        hashStr << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(hash[i]);
    }
    return hashStr.str();
}

void str2sha(const std::string &hash_str, unsigned char *out) {
    // Starting from 1 bc of the C++ string 0 and last chars.
    for (int i=1; i<HASH_SIZE*2; i+=2) {
        std::istringstream converter(hash_str.substr(i, 2));
        unsigned int value;
        converter >>  std::hex >> value;
        out[int(i/2)] = static_cast<unsigned char>(value);
    }
}
