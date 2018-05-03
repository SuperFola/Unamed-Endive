#include<iostream>
#include "../../debug.hpp"

#include "creatures_loader.hpp"

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
                if (std::string(File.cFileName) != "." && std::string(File.cFileName) != "..")
                    files.push_back(std::string(File.cFileName));
        } while (FindNextFile(hSearch, &File));

        FindClose(hSearch);
    }
    #endif // PLATFORM_WIN

    #ifdef PLATFORM_POSIX
    DIR* rep = opendir(directory.data());

    if (rep != NULL)
    {
        struct dirent* ent;

        while ((ent = readdir(rep)) != NULL)
        {
            if (std::string(ent->d_name) != "." && std::string(ent->d_name) != "..")
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
    this->files = list_files(this->folder);
}

bool CreaturesLoader::load_next()
{
    bool done = false;
    std::string file = this->files[this->current];

    sf::Image image;

    if (file != "." && file != "..")
    {
        if (!image.loadFromFile(this->folder + "/" + file))
            DebugLog(SH_ERR, "Unable to open " << this->folder + "/" + file);
        else
        {
            image.createMaskFromColor(sf::Color(255, 0, 255, 255));
            sf::Texture texture;
            texture.loadFromImage(image);
            texture.setSmooth(true);
            this->textures.add(file, texture);
        }
    }


    this->current++;  // increment the cursor

    if (this->current == this->files.size())
        done = true;  // we tell the caller of this method that we are done
    if (done)
        DebugLog(SH_OK, "Finished loading all the creatures (images)");

    return done;
}

sf::Texture& CreaturesLoader::get(const std::string& name)
{
    return this->textures.get(name);
}
