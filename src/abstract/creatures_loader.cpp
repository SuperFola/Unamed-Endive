#include<iostream>

#include "creatures_loader.hpp"
#include "../constants.hpp"

#ifdef PLATFORM_WIN
#include <windows.h>
#endif // PLATFORM_WIN

#ifdef PLATFORM_POSIX
#include <dirent.h>
#endif // PLATFORM_POSIX

std::vector<std::string> list_files(const std::string& directory)
{
    std::vector<std::string> files;

    #ifdef PLATFORM_WIN
    WIN32_FIND_DATA File;
    HANDLE hSearch;

    hSearch = FindFirstFile((directory + "/*.*").data(), &File);
    if (hSearch != INVALID_HANDLE_VALUE)
    {
        do {
                if (File.cFileName != "." && File.cFileName != "..")
                    files.push_back(std::string(File.cFileName));
        } while (FindNextFile(hSearch, &File));

        FindClose(hSearch);
    }
    #endif // PLATFORM_WIN

    #ifdef PLATFORM_POSIX
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
    #endif // PLATFORM_POSIX

    return files;
}

// public
CreaturesLoader::CreaturesLoader()
{

}

bool CreaturesLoader::load()
{
    std::vector<std::string> files = list_files(this->folder);

    for (const auto& file: files)
    {
        sf::Image image;

        std::cout << "Loading " << this->folder + "/" + file << std::endl;
        if (!image.loadFromFile(this->folder + "/" + file))
        {
            std::cout << "Unable to open " << this->folder + "/" + file << std::endl;
        }

        image.createMaskFromColor(sf::Color(255, 0, 255, 255));
        sf::Texture texture;
        texture.loadFromImage(image);

        this->textures.addTexture(file, texture);
    }

    std::cout << "Finished loading creatures sprites" << std::endl;
}

sf::Texture& CreaturesLoader::get(const std::string& name)
{
    return this->textures.getTexture(name);
}
