#include<iostream>

#include "creatures_loader.hpp"
#include "functions.hpp"

// public
CreaturesLoader::CreaturesLoader()
{

}

bool CreaturesLoader::load()
{
    std::vector<std::string> files = glob(this->folder);

    for (const auto& file: files)
    {
        sf::Image image;

        std::cout << "Loading " << file << std::endl;
        if (!image.loadFromFile(file))
        {
            std::cout << "Unable to open " << file << std::endl;
        }

        image.createMaskFromColor(sf::Color(255, 0, 255, 255));
        sf::Texture texture;
        texture.loadFromImage(image);

        this->textures.addTexture(file, texture);
        this->sprites.push_back(sf::Sprite(this->textures.getTexture(file)));
    }

    std::cout << "Finished loading creatures sprites" << std::endl;
}
