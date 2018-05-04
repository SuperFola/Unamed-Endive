#ifndef DEF_ABS_FUNC
#define DEF_ABS_FUNC

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <SFML/System/String.hpp>
#include <SFML/Graphics/Font.hpp>

#include "../utf8.h"

#include "../constants.hpp"
#ifdef PLATFORM_WIN
    #include <windows.h>
#endif // PLATFORM_WIN
#ifdef PLATFORM_POSIX
     #include <sys/types.h>
    #include <sys/stat.h>
#endif
#include "../network/network.hpp"

#define CLEAR_PTR_VECT(v) for(auto it=v.begin(); it != v.end(); ++it) {delete (*it);} v.clear();
#define CLEAR_PTR_MAP(m) for(auto it=m.begin(); it != m.end(); ++it) {delete (m->second)} m.clear();

template<typename T>
T pop(std::vector<T>* obj, int i) {
    T element {};

    if (0 <= i && i < obj->size())
    {
        element = (*obj)[i];
        obj->erase(obj->begin() + i);
    }
    else if (i < 0 && (-i) < obj->size())
    {
        element = (*obj)[obj->size() + i];
        obj->erase(obj->begin() + obj->size() + i);
    }

    return element;
}

template<typename T>
std::string to_string(const T& n)
{
    std::ostringstream stm ;
    stm << n ;
    return stm.str() ;
}

bool is_file_existing(const std::string& file);

sf::String wrapText(sf::String, int, const sf::Font&, int, bool bold=false);

std::string convert_type_to_str(int);

void fix_utf8_string(std::string&);

#endif // DEF_ABS_FUNC
