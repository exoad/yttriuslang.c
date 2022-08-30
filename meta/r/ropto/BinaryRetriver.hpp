#ifndef ropto_BinaryRetriver_hpp
#define ropto_BinaryRetriver_hpp

#include <cstdint>

namespace ropto
{
    
    template<class T>
    union binary_retriver
    {
    public:
        T value;
        uint8_t bytes[sizeof(T)];
        binary_retriver() {}
        binary_retriver(const T& _value): value(_value) {
            uint8_t bytes[sizeof(value)];
        }
    };

}

#endif
