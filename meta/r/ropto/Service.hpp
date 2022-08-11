//
//  Service.h
//  ropto
//
//  Created by Kimmy  on 3/15/16.
//  Copyright (c) 2016 TDCA. All rights reserved.
//

#ifndef __ropto__Service__
#define __ropto__Service__

#include <memory>
#include <functional>
#include <cassert>

#include "ByteStream.hpp"
#include "Serializer.hpp"
#include "SerializerSupport.hpp"

namespace ropto
{
    
    struct message_buf
    {
        unsigned int type_id;
        bytes_t bytes;
        
        std::unique_ptr<byte_stream> stream() const
        {
            auto stream = std::make_unique<byte_stream>();
            stream->iterate() = bytes;
            return stream;
        }
    };
    
    template<class T>
    message_buf make_message(T& object)
    {
        byte_stream stream;
        write(stream, object);
        
        return message_buf {T::type_id, stream.iterate()};
    }
    
    template<>
    class serializer<message_buf>
    {
    public:
        static message_buf from_bytes(byte_stream& stream)
        {
            message_buf mb;
            read(stream, mb.type_id, mb.bytes);
            return mb;
        }
        
        static void to_bytes(const message_buf& mb, byte_stream& stream)
        {
            write(stream, mb.type_id, mb.bytes);
        }

    };
    
    template<class Tin, class Tout = Tin>
    class service
    {
        static constexpr unsigned int type_in = Tin::type_id;
        static constexpr unsigned int type_out = Tout::type_id;
        
        std::function<void(Tin&, Tout&)> handler;
        
    public:
        
        service(std::function<void(Tin&, Tout&)> fn): handler(fn) {};
        service(const service&) = delete;
        
        message_buf process(const message_buf& mb)
        {
            assert(mb.type_id == type_in);
            Tout out {};
            auto in = read<Tin>(*mb.stream());
            handler(in, out);
            
            return make_message(out);
        }
    };
    
    template<class Tin, class Tout = Tin>
    std::shared_ptr<service<Tin, Tout>> make_service(std::function<void(Tin&, Tout&)> fn)
    {
        return std::make_shared<service<Tin, Tout>>(fn);
    }
    
}

#endif /* defined(__ropto__Service__) */
