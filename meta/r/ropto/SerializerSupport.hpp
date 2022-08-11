
#ifndef ropto_SerializerSupport_hpp
#define ropto_SerializerSupport_hpp

#include <iterator>
#include <map>
#include <algorithm>

#include "BinaryRetriver.hpp"
#include "Serializer.hpp"
#include "Optional.hpp"


namespace ropto
{
    template<class T>
    class serializer<T, true, false, false>
    {
    public:
        static T from_bytes(byte_stream& stream)
        {
            binary_retriver<T> retriver;
            for(int i = 0; i < sizeof(retriver); i++)
                retriver.bytes[i] = stream.fetch();
            return retriver.value;
        }
        static void to_bytes(const T& value, byte_stream& stream)
        {
            binary_retriver<T> retriver{value};
            for (auto byte: retriver.bytes)
                stream.append(byte);
        }
    };
    
    template<class Integral>
    class serializer<Integral, true, true, false>
    {
    public:
        static Integral from_bytes(byte_stream& stream)
        {
            std::decay_t<Integral> value = 0;
            for(size_t i = sizeof(Integral); i > 0; i--)
            {
                value += static_cast<Integral>(stream.fetch()) << (8*(i-1));
            }
            return value;
        }
        
        static void to_bytes(const Integral& value, byte_stream& stream)
        {
            for(size_t i = sizeof(Integral); i > 0; i--)
            {
                uint8_t byte = (value >> (8*(i-1))) & 0xFF;
                stream.append(byte);
            }
        }
    };
    
    template<class T, int size=sizeof(T)>
    union Integral;
    
    template<class T>
    union Integral<T, 1> {int8_t i; T t;};
    
    template<class T>
    union Integral<T, 2> { int16_t i; T t; };
    
    template<class T>
    union Integral<T, 4> { int32_t i; T t; };
    
    template<class T>
    union Integral<T, 8> { int64_t i; T t; };
    
    template<class T>
    union Integral<T, 16> { __int128_t i; T t; };
    
    template<class Floating>
    class serializer<Floating, true, false, true>
    {
        typedef Integral<Floating> Int;
    public:
        static Floating from_bytes(byte_stream& stream)
        {
            Int value {};
            read(value.i, stream);
            return value.t;
        }
        static void to_bytes(const Floating& value, byte_stream& stream)
        {
            Int ivalue {};
            ivalue.t = value;
            write(ivalue.i, stream);
        }
    };
    
    template<class T>
    class serializer<optional<T>, false, false, false>
    {
    public:
        static optional<T> from_bytes(byte_stream& stream)
        {
            T value;
            bool valid = false;
            read(stream, valid);
            if (valid)
                read(stream, value);
            return valid ? optional<T>{value} : optional<T> {};
        }
        
        static void to_bytes(const optional<T>& value, byte_stream& stream)
        {
            write(static_cast<bool>(value), stream);
            if(value)
                write(value.value(), stream);
        }
    };
    
    template<class Container>
    struct OutIter
    {
        typedef std::true_type is_back_insertable;
        typedef std::back_insert_iterator<Container> type;
    };
    
    template<class K, class V>
    struct OutIter<std::map<K, V>>
    {
        typedef std::false_type is_back_insertable;
        typedef std::insert_iterator<std::map<K, V>> type;
    };
    
    template<class Container>
    using OutIter_t = typename OutIter<Container>::type;
    
    template<class Container>
    OutIter_t<Container> InserterOf(Container& container, std::false_type)
    {
        return std::inserter(container, end(container));
    }
    
    template<class Container>
    OutIter_t<Container> InserterOf(Container& container, std::true_type)
    {
        return std::back_inserter(container);
    }
    
    template<class Container>
    OutIter_t<Container> Inserter(Container& container)
    {
        return InserterOf(container, typename OutIter<Container>::is_back_insertable {});
    }
    
    template<class Container>
    class serializer<Container, false, false, false>
    {
    public:
        static Container from_bytes(byte_stream& stream)
        {
            Container container;
            auto count = read<size_t>(stream);
            std::generate_n(Inserter(container), count, [&stream]()
            {
                return read<typename Container::value_type>(stream);
            });
            
            return container;
        }
        
        static void to_bytes(const Container& container, byte_stream& stream)
        {
            write(container.size(), stream);
            for (auto item: container)
                write(item, stream);
        }
    };
    
    template<class K, class V>
    class serializer<std::pair<K, V>, false, false, false>
    {
    public:
        static std::pair<K, V> from_bytes(byte_stream& stream)
        {
            auto first = read<std::decay_t<K>>(stream);
            auto second = read<V>(stream);
            return std::make_pair(first, second);
        }
        
        static void to_bytes(const std::pair<K, V>& pair, byte_stream& stream)
        {
            write(pair.first, stream);
            write(pair.second, stream);
        }
    };

}

#endif