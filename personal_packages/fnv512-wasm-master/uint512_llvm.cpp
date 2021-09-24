#include "uint512_llvm.hpp"

bool uint512_littleendian_uncached();

const bool isLittleEndian = uint512_littleendian_uncached();

bool uint512_littleendian_uncached()
{
    const uint16_t number = 0x1;
    const void* p = &number;
    const uint8_t* bytePtr = (const uint8_t*) p;
    return (bytePtr[0] != 0);
}
bool uint512_littleendian()
{
    return isLittleEndian;
}

void uint512_debug_(const char* prefix, const uint512_t& u512){
        uint512_u hash_convert;
        hash_convert.u512 = u512;

        std::cout<<" little-endian? "<<uint512_littleendian()<<std::endl;
        std::cout<<" debug "<<prefix<<" u64[";
        for(int i=0;i<8;i++) std::cout<< hash_convert.u64[i] <<" ";
        std::cout<<"]"<<std::endl;

        std::cout<<" debug "<<prefix<<" u8[";
        for(int i=0;i<64;i++) std::cout<< (unsigned int) hash_convert.u8[i] <<" ";
        std::cout<<"]"<<std::endl;
}

