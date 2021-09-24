#include <boost/multiprecision/cpp_int.hpp>
#include <boost/atomic/detail/bitwise_cast.hpp>
#include <stdio.h>

using namespace boost::multiprecision;
using namespace boost::multiprecision::literals;
using namespace boost::atomics::detail;


constexpr uint512_t fnv_prime = 0x00000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000000000000000000000000157_cppui512;

char nibble2hex(unsigned char nibble) {
  if (nibble < 10)
    return '0' + nibble;
  else
    return 'a' + nibble - 10;
}

void fnv0(const char* data, size_t len, char* hexdigest) {//hexdigest should be 64*2 chars long

  // implementation : https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function#FNV-0_hash_(deprecated)
  uint512_t hash = 0;

  for (size_t i = 0; i < len; i++) {
    // multiplication by 2^344 + 2^8 + 0x57
    hash *= fnv_prime;


    // xor with a byte of data
    hash ^= data[i];


  }

  for (unsigned int i = 0; i < 128; i++) {
    unsigned char hashc1 = bitwise_cast<unsigned char>( hash );
//    unsigned char hashc2 = static_cast<unsigned char>( hash );
//    std::cout<<" hash to char: "<<(unsigned int)hashc1<<" "<<(unsigned int)hashc2<<std::endl;

//    std::cout<<" hash to char: "<<(unsigned int)static_cast<unsigned char>(hash)<<std::endl;
    hexdigest[127 - i] = nibble2hex( hashc1 & 0xf);
//    hexdigest[127 - i] = nibble2hex(hash & (unsigned char) 0xff);
    hash >>= 4;
  }
}

int main(){
const char* data = "chongo <Landon Curt Noll> /\\../\\";
char* hexdigest = (char*) malloc(sizeof(char)*129);
hexdigest[128]=0;

fnv0(data, strlen(data), hexdigest);

printf("%s",hexdigest);

free(hexdigest);
return 0;
}

