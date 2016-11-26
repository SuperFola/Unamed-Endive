#include "functions.hpp"

bool is_file_existing(const std::string& file)
{
    std::ifstream reader(file.c_str());
    return !reader.fail();
}

bool create_directory(const std::string& name)
{
    #ifdef PLATFORM_WIN
        return bool(CreateDirectory(name.c_str(), NULL));
    #endif
    #ifdef PLATFORM_POSIX
         return bool(mkdir(name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH));
    #endif
}
