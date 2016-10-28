#ifndef DEF_ABS_FUNC
#define DEF_ABS_FUNC

#include <vector>
#include <string>
#include <sstream>

#include "../constants.hpp"

#ifdef PLATFORM_WIN
#include <windows.h>
#elifdef PLATFORM_POSIX
#include <dirent.h>
#endif // PLATFORM_WIN

std::vector<std::string> glob(const std::string& directory)
{
    std::vector<std::string> files;

    #ifdef PLATFORM_WIN
    WIN32_FIND_DATA File;
    HANDLE hSearch;

    hSearch = FindFirstFile(directory + "/*.*", &File);
    if (hSearch != INVALID_HANDLE_VALUE)
    {
        do {
                files.push_back(std::string(File.cFileName));
        } while (FindNextFile(hSearch, &File));

        FindClose(hSearch);
    }
    #elifdef PLATFORM_POSIX
    DIR* rep = opendir(directory);

    if (rep != NULL)
    {
        struct dirent* ent;

        while ((ent = readdir(rep)) != NULL)
        {
            files.push_back(ent->d_name);
        }

        closedir(rep);
    }
    #endif // PLATFORM_WIN

    return files;
}

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

#endif // DEF_ABS_FUNC
