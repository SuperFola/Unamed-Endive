#include <iostream>
#include "utf8.hpp"

std::string fix_my_utf8(const char* base)
{
    for (int i=0; base[i] != '\0'; ++i)
    {
        unsigned char c = base[i];
        std::cout << c << " " << static_cast<int>(c) << std::endl;
    }

    std::string out = "";
    for (int i=0; base[i] != '\0'; ++i)
    {
        unsigned char c = base[i];

        if (c < 0x80)  // 128
            out += c;
        else
        {
            unsigned char next = (base[i + 1] != '\0') ? base[++i] : c;
            unsigned char n = ((c >> 6) | 0xc0) + ((next & 0x3f) | 0x80);
            out += n;
        }
    }
    // fix_utf8_string(out);
    std::cout << "1 >>> " << out << std::endl;
    return out;
}
