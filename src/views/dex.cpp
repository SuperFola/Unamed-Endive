#include <iostream>

#include "dex.hpp"
#include "../constants.hpp"

#define __X event.mouseButton.x
#define __Y event.mouseButton.y

// public
DexView::DexView() :
    View(DEX_VIEW_ID)
    , selected(0)
    , loaded(false)
{

}

bool DexView::load()
{
    // loading textures
    sf::Texture background;
    if (!background.loadFromFile("assets/gui/fd_indexer.png"))
        return false;
    this->textures.add("background", background);

    // setting sprites
    this->sprites[this->BCKG] = sf::Sprite(this->textures.get("background"));

    // setting pos
    this->sprites[this->BCKG].setPosition(20.0f, 20.0f);

    // texts
    if (!this->font.loadFromFile("assets/fonts/pkmnemn.ttf"))
        return false;
    this->text.setFont(this->font);
    this->text.setString("Dexeur");
    this->text.setCharacterSize(24);
    this->text.setFillColor(sf::Color::Black);
    this->text.setPosition(WIN_W / 2 - this->text.getGlobalBounds().width / 2, 30.0f);

    return true;
}

void DexView::render(sf::RenderWindow& window)
{
    window.draw(this->sprites[this->BCKG]);
    window.draw(this->text);
    this->draw_content(window);
}

int DexView::process_event(sf::Event& event, sf::Time elapsed)
{
    int new_view = -1;

    switch(event.type)
    {
    case sf::Event::KeyPressed:
        switch(event.key.code)
        {
        case sf::Keyboard::Escape:
            new_view = LAST_VIEW_ID;
            break;

        default:
            break;
        }
        break;

    case sf::Event::MouseButtonPressed:
        switch(event.mouseButton.button)
        {
        case sf::Mouse::Button::Left:
            // selecting a creature
            if (__X >= 30 && __X <= WIN_W - 30 && __Y >= this->text.getPosition().y + 84.0f && __Y <= WIN_H - 30)
            {
                int ry = (__Y - (this->text.getPosition().y + 84.0f)) / 64;
                if (ry >= 0 && ry < 8)
                {
                    // ok the selected creature is in range
                    this->selected += ry;
                    this->selected %= this->dex_content.size();
                }
            }
            break;

        default:
            break;
        }
        break;

    case sf::Event::MouseWheelScrolled:
        this->selected -= event.mouseWheelScroll.delta;
        if (this->selected < -1)
            this->selected = -1;
        if (this->selected > this->dex_content.size())
            this->selected = 0;
        break;

    default:
        break;
    }

    return new_view;
}

void DexView::update(sf::RenderWindow& window, sf::Time elapsed)
{
}

int DexView::index_of(const std::string& name)
{
    int i = 0;

    for (int i=0; i < this->dex_content.size(); i++)
    {
        if (this->dex->getInfo(std::get<2>(this->dex_content[i])).evolution == name)
            return i;
    }

    return i;
}

void DexView::draw_content(sf::RenderWindow& window)
{
    for (int i=this->selected; i < this->selected + 9; i++)
    {
        std::get<0>(this->dex_content[i % this->dex_content.size()]).setPosition(30.0f, this->text.getPosition().y + 84.0f + (i - this->selected) * 64.0f);
        window.draw(std::get<0>(this->dex_content[i % this->dex_content.size()]));
        if (i == this->selected && this->dex->getInfo(std::get<2>(this->dex_content[i % this->dex_content.size()])).viewed)
            window.draw(std::get<1>(this->dex_content[i % this->dex_content.size()]));
    }
}

void DexView::add_dex(Dex* dex)
{
    this->dex = dex;
}

void DexView::add_crealoader(CreaturesLoader* creaload)
{
    this->crealoader = creaload;
    this->loaded = true;
    // load the content of the dex by creating all the sf::Text
    this->load_dex_content();
}

void DexView::load_dex_content()
{
    if (!this->loaded)
        goto hell_again;

    {
        // we must ensure it is empty each time we use this method
        this->dex_content.clear();

        struct DexInfo crea;

        // variables for formating
        std::string stext;
        std::string stype;
        std::string vu;
        std::string capture;
        std::string evolution;

        // magic variables
        int id = 0;
        std::string current;

        current = this->dex->getNext();
        while (current != "")
        {
            std::tuple<sf::Text, sf::Sprite, std::string> content;

            crea = this->dex->getInfo(current);

            sf::Text _text;
            _text.setFont(this->font);
            _text.setCharacterSize(20);
            _text.setFillColor(sf::Color::Black);

            sf::Sprite _sprite;
            _sprite.setTexture(this->crealoader->get(crea.file));
            float factor = 180.0f / this->crealoader->get(crea.file).getSize().y;
            _sprite.setScale(factor, factor);

            switch (crea.type)
            {
            case Type::NORMAL:
                stype = "Normal";
                break;

            case Type::FIRE:
                stype = "Feu";
                break;

            case Type::WATER:
                stype = "Eau";
                break;

            case Type::GRASS:
                stype = "Plante";
                break;

            case Type::FLYING:
                stype = "Vol";
                break;

            case Type::FIGHTING:
                stype = "Combat";
                break;

            case Type::POISON:
                stype = "Poison";
                break;

            case Type::ELECTRIC:
                stype = "Electrique";
                break;

            default:
                stype = "???";
                break;
            }
            vu = (crea.viewed) ? "oui" : "non";
            capture = (crea.captured) ? "oui" : "non";
            evolution = (crea.evolution != "") ? crea.evolution : "Aucune";

            stext = current
                + ", " + stype
                + "\nNiveau " + to_string<int>(crea.stade)
                + ", Evolution : " + evolution
                + "\nVu : " + vu
                + ", Capturé : " + capture;

            _text.setString(stext);

            _sprite.setPosition(WIN_W - 30.0f - _sprite.getGlobalBounds().width, this->text.getPosition().y + 104.0f);

            std::get<0>(content) = _text;
            std::get<1>(content) = _sprite;
            std::get<2>(content) = current;

            this->dex_content.push_back(content);

            current = this->dex->getNext();
            id++;
        }
    }

    hell_again:;
}
