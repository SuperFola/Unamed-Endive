#include <iostream>
#include "utf8.hpp"
#include "../../debug.hpp"

std::wstring fix_my_utf8(const char* base)
{
    std::string out = "";
    for (int i=0; base[i] != '\0'; ++i)
    {
        unsigned char c = base[i];
        if (c < 0x80)  // 128
            out += c;
        else
        {
            unsigned char next = (base[i + 1] != '\0') ? base[++i] : c;
            unsigned char calculated = (c & 0b11110000) | next;
            out += calculated;
        }
    }
    // fix_utf8_string(out);
    DebugLog(SH_SPE, out);
    std::wstring out3(out.size(), L' ');
    out3.resize(std::mbstowcs(&out3[0], out.c_str(), out.size()));

    return out3;
}
