#include <iostream>

#include "dex.hpp"
#include "../constants.hpp"

#define __X event.mouseButton.x
#define __Y event.mouseButton.y

// public
DexView::DexView() :
    View(DEX_VIEW_ID)
    , displaying_crea(true)
    , selected(-1)
{

}

bool DexView::load()
{
    // loading textures
    sf::Texture background;
    if (!background.loadFromFile("assets/gui/fd_indexer.png"))
        return false;
    this->textures.addTexture("background", background);

    sf::Texture button;
    if (!button.loadFromFile("assets/gui/fd_bouton_types_indexeur.png"))
        return false;
    this->textures.addTexture("btn_types", button);

    sf::Texture crea_btn;
    if (!crea_btn.loadFromFile("assets/gui/fd_bouton_creatures_indexeur.png"))
        return false;
    this->textures.addTexture("btn_crea", crea_btn);

    // setting sprites
    this->sprites[this->BCKG] = sf::Sprite(this->textures.getTexture("background"));
    this->sprites[this->TYPES_BTN] = sf::Sprite(this->textures.getTexture("btn_types"));
    this->sprites[this->CREA_BTN] = sf::Sprite(this->textures.getTexture("btn_crea"));

    // setting pos
    this->sprites[this->BCKG].setPosition(20.0f, 20.0f);
    this->sprites[this->TYPES_BTN].setPosition(529.0f, 30.0f);
    this->sprites[this->CREA_BTN].setPosition(529.0f, 30.0f);

    // texts
    if (!this->font.loadFromFile("assets/fonts/pkmnemn.ttf"))
        return false;
    this->text.setFont(this->font);
    this->text.setString("Dexeur: Créatures");
    this->text.setCharacterSize(24);
    this->text.setColor(sf::Color::White);
    this->text.setPosition(WIN_W / 2 - this->text.getGlobalBounds().width / 2, 30.0f);

    return true;
}

void DexView::render(sf::RenderWindow& window)
{
    window.draw(this->sprites[this->BCKG]);
    window.draw(this->text);
    if (this->displaying_crea)
        window.draw(this->sprites[this->TYPES_BTN]);
    else
        window.draw(this->sprites[this->CREA_BTN]);
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
            if (__X >= 529 && __X <= 610 && __Y >= 30 && __Y <= 53)
            {
                this->displaying_crea = !this->displaying_crea;
                if (this->displaying_crea)
                    this->text.setString("Dexeur: Créatures");
                else
                    this->text.setString("Dexeur: Types");
            }
            break;

        default:
            break;
        }
        break;

    case sf::Event::MouseWheelScrolled:
        this->selected += event.mouseWheelScroll.delta;
        if (this->selected < -1)
            this->selected = -1;
        break;

    default:
        break;
    }

    return new_view;
}

void DexView::update(sf::RenderWindow& window, sf::Time elapsed)
{

}

void DexView::draw_content(sf::RenderWindow& window)
{
    if (this->displaying_crea)
    {
        for (int i=0; i < this->dex_content.size(); i++)
        {
            window.draw(std::get<0>(this->dex_content[i]));
            if (i == this->selected && this->dex->getInfo(std::get<2>(this->dex_content[i])).viewed)
                window.draw(std::get<1>(this->dex_content[i]));
        }
    }
    else
    {
        // ...
    }
}

void DexView::add_dex(Dex* dex)
{
    this->dex = dex;
}

void DexView::add_crealoader(CreaturesLoader* creaload)
{
    this->crealoader = creaload;
    // load the content of the dex by creating all the sf::Text
    this->load_dex_content();
}

void DexView::load_dex_content()
{
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
        _text.setColor(sf::Color::Black);

        sf::Sprite _sprite;
        _sprite.setTexture(this->crealoader->get(crea.file));
        float factor = 200.0f / this->crealoader->get(crea.file).getSize().y;
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
        _text.setPosition(30.0f, this->text.getPosition().y + 20.0f + id * 64.0f);

        _sprite.setPosition(WIN_W - 20.0f - _sprite.getGlobalBounds().width, this->text.getPosition().y + 40.0f);

        std::get<0>(content) = _text;
        std::get<1>(content) = _sprite;
        std::get<2>(content) = current;

        this->dex_content.push_back(content);

        current = this->dex->getNext();
        id++;
    }
}
