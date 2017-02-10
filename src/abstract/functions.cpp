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

// from https://github.com/LiquidHelium
sf::String wrapText(sf::String str, int width, const sf::Font& font, int characterSize, bool bold)
{
    int currentOffset = 0;
    bool firstWord = true;
    std::size_t wordBegining = 0;

    for (std::size_t pos(0); pos < str.getSize(); ++pos)
    {
        auto currentChar = str[pos];
        if (currentChar == '\n')
        {
            currentOffset = 0;
            firstWord = true;
            continue;
        }
        else if (currentChar == ' ')
        {
            wordBegining = pos;
            firstWord = false;
        }

        auto glyph = font.getGlyph(currentChar, characterSize, bold);
        currentOffset += glyph.advance;

        if (!firstWord && currentOffset > width)
        {
            pos = wordBegining;
            str[pos] = '\n';
            str.insert(pos, "\n");
            firstWord = true;
            currentOffset = 0;
        }
    }

    return str;
}
