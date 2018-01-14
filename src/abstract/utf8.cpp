#include <iostream>
#include "utf8.hpp"

std::wstring fix_my_utf8(const char* base)
{
    std::string out = "";
    for (int i=0; base[i] != '\0'; ++i)
    {
        wchar_t c = base[i];

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

    const std::size_t cSize = strlen(out.c_str()) + 1;
    wchar_t* wc = new wchar_t[cSize];
    mbstowcs(wc, out.c_str(), cSize);

    std::wstring out2(wc);
    for (std::size_t i=0; i < cSize; wc[i++] = L'\0');
    delete[] wc;
    std::wstring out3 = L"";

    for (std::size_t i=0; i != out2.size(); ++i)
    {
        /*
        à   é   è   ñ   ë   ê   â   ù   À   È    É   Ñ   Ë   Ê    Â   Ù
        376 168 167 176 170 169 161 184 127 8225 710 144 352 8240 129 732
        */
        switch(static_cast<int>(out2[i])) {
        case  376: out3 += L'à';    break;
        case  168: out3 += L'é';    break;
        case  167: out3 += L'è';    break;
        case  176: out3 += L'ñ';    break;
        case  170: out3 += L'ë';    break;
        case  169: out3 += L'ê';    break;
        case  161: out3 += L'â';    break;
        case  184: out3 += L'ù';    break;
        case  127: out3 += L'À';    break;
        case 8225: out3 += L'È';    break;
        case  710: out3 += L'É';    break;
        case  144: out3 += L'Ñ';    break;
        case  352: out3 += L'Ë';    break;
        case 8240: out3 += L'Ê';    break;
        case  129: out3 += L'Â';    break;
        case  732: out3 += L'Ù';    break;
        default  : out3 += out2[i]; break;
        }
    }

    return out3;
}
