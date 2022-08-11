
#ifndef ropto_Base64_hpp
#define ropto_Base64_hpp

#include <string>
#include <vector>

namespace ropto
{
    std::string base64_encode(const std::vector<uint8_t>& bytes);
    
    std::vector<uint8_t> base64_decode(const std::string& input);
        
    constexpr static char base64map[] = ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
    
    constexpr static char pad_char = ('=');
    
    inline std::string base64_encode(const std::vector<uint8_t>& bytes)
    {
        std::string encoded;
        encoded.reserve(((bytes.size()/3) + (bytes.size() % 3 > 0)) * 4);
        uint32_t temp;
        auto cursor = bytes.begin();
        for(size_t idx = 0; idx < bytes.size()/3; idx++)
        {
            temp  = (*cursor++) << 16;
            temp += (*cursor++) << 8;
            temp += (*cursor++);
            encoded.append(1,base64map[(temp & 0x00FC0000) >> 18]);
            encoded.append(1,base64map[(temp & 0x0003F000) >> 12]);
            encoded.append(1,base64map[(temp & 0x00000FC0) >> 6 ]);
            encoded.append(1,base64map[(temp & 0x0000003F)      ]);
        }
        switch(bytes.size() % 3)
        {
            case 1:
                temp  = (*cursor++) << 16;
                encoded.append(1,base64map[(temp & 0x00FC0000) >> 18]);
                encoded.append(1,base64map[(temp & 0x0003F000) >> 12]);
                encoded.append(2,pad_char);
                break;
            case 2:
                temp  = (*cursor++) << 16;
                temp += (*cursor++) << 8;
                encoded.append(1,base64map[(temp & 0x00FC0000) >> 18]);
                encoded.append(1,base64map[(temp & 0x0003F000) >> 12]);
                encoded.append(1,base64map[(temp & 0x00000FC0) >> 6 ]);
                encoded.append(1,pad_char);
                break;
        }
        return encoded;
    }
    
    inline std::vector<uint8_t> base64_decode(const std::string& input)
    {
        if (input.length() % 4)
            throw std::runtime_error("Non-Valid base64!");
        size_t padding = 0;
        if (input.length())
        {
            if (input[input.length()-1] == pad_char)
                padding++;
            if (input[input.length()-2] == pad_char)
                padding++;
        }
        
        std::vector<uint8_t> decoded;
        decoded.reserve(((input.length()/4)*3) - padding);
        uint32_t temp=0;
        auto cursor = input.begin();
        while (cursor < input.end())
        {
            for (size_t position = 0; position < 4; position++)
            {
                temp <<= 6;
                if       (*cursor >= 0x41 && *cursor <= 0x5A)
                    temp |= *cursor - 0x41;
                else if  (*cursor >= 0x61 && *cursor <= 0x7A)
                    temp |= *cursor - 0x47;
                else if  (*cursor >= 0x30 && *cursor <= 0x39)
                    temp |= *cursor + 0x04;
                else if  (*cursor == 0x2B)
                    temp |= 0x3E;
                else if  (*cursor == 0x2F)
                    temp |= 0x3F;
                else if  (*cursor == pad_char)
                {
                    switch( input.end() - cursor )
                    {
                        case 1:
                            decoded.push_back((temp >> 16) & 0x000000FF);
                            decoded.push_back((temp >> 8 ) & 0x000000FF);
                            return decoded;
                        case 2:
                            decoded.push_back((temp >> 10) & 0x000000FF);
                            return decoded;
                        default:
                            throw std::runtime_error("Invalid Padding in Base 64!");
                    }
                }  else
                    throw std::runtime_error("Non-Valid Character in Base 64!");
                cursor++;
            }
            decoded.push_back((temp >> 16) & 0x000000FF);
            decoded.push_back((temp >> 8 ) & 0x000000FF);
            decoded.push_back((temp      ) & 0x000000FF);
        }
        return decoded;
    }
}


#endif /* defined(__ropto__Base64__) */
