
#ifndef ropto_Serializer_hpp
#define ropto_Serializer_hpp

#include "ByteStream.hpp"

#include <type_traits>

namespace ropto
{
    template<class T, bool pod = std::is_pod<T>::value,
                      bool integral = std::is_integral<T>::value,
                      bool floating = std::is_floating_point<T>::value>
    class serializer;
    
    template<class T>
    T read(byte_stream& stream)
    {
        return serializer<T>::from_bytes(stream);
    }
    
    template<class T>
    void write(const T& value, byte_stream& stream)
    {
        serializer<T>::to_bytes(value, stream);
    }
    
    template<class T>
    void read(T& value, byte_stream& stream)
    {
        value = read<T>(stream);
    }
    
    template<class... Args>
    void write(byte_stream& stream,Args&... args)
    {
        (stream << ... << args);
    }
    
    template<class... Args>
    void read(byte_stream& stream, Args&... args)
    {
        (stream >> ... >> args);
    }
    
    template<class T>
    byte_stream& operator<<(byte_stream& stream, const T& value)
    {
        write(value, stream);
        return stream;
    }
    
    template<class T>
    byte_stream& operator>>(byte_stream& stream, T& value)
    {
        read(stream, value);
        return stream;
    }

}

#endif
