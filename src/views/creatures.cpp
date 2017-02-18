#include <iostream>

#include "creatures.hpp"
#include "../constants.hpp"

#define __X event.mouseButton.x
#define __Y event.mouseButton.y

// public
CreaView::CreaView() :
    View(MYCREATURES_VIEW_ID)
    , displaying_crea(true)
{

}

bool CreaView::load()
{
    // loading textures
    sf::Texture background;
    if (!background.loadFromFile("assets/gui/equip/fd_equip.png"))
        return false;
    this->textures.addTexture("background", background);

    sf::Texture bckg_pc;
    if (!bckg_pc.loadFromFile("assets/gui/equip/fd_pc.png"))
        return false;
    this->textures.addTexture("background_pc", bckg_pc);

    sf::Texture btn_pc;
    if (!btn_pc.loadFromFile("assets/gui/equip/pc.png"))
        return false;
    this->textures.addTexture("btn_pc", btn_pc);

    sf::Texture btn_crea;
    if (!btn_crea.loadFromFile("assets/gui/equip/team.png"))
        return false;
    this->textures.addTexture("btn_crea", btn_crea);

    // setting sprites
    this->sprites[this->BCKG] = sf::Sprite(this->textures.getTexture("background"));
    this->sprites[this->BCKG_PC] = sf::Sprite(this->textures.getTexture("background_pc"));
    this->sprites[this->BTN_PC] = sf::Sprite(this->textures.getTexture("btn_pc"));
    this->sprites[this->BTN_CREA] = sf::Sprite(this->textures.getTexture("btn_crea"));

    // setting pos
    this->sprites[this->BCKG].setPosition(0.0f, 0.0f);
    this->sprites[this->BCKG_PC].setPosition(0.0f, 0.0f);
    this->sprites[this->BTN_PC].setPosition(238.0f, 10.0f);
    this->sprites[this->BTN_CREA].setPosition(238.0f, 10.0f);

    // texts
    if (!this->font.loadFromFile("assets/fonts/pkmnemn.ttf"))
        return false;

    this->cnam.setFont(this->font);
    this->cnam.setColor(sf::Color::Black);
    this->cnam.setCharacterSize(24);
    this->cnam.setPosition(388.0f, 156.0f);

    this->clev.setFont(this->font);
    this->clev.setColor(sf::Color::Black);
    this->clev.setCharacterSize(24);
    this->clev.setPosition(565.0f, 156.0f);

    this->ctyp.setFont(this->font);
    this->ctyp.setColor(sf::Color::Black);
    this->ctyp.setCharacterSize(18);
    this->ctyp.setPosition(424.0f, 345.0f);

    this->csta.setFont(this->font);
    this->csta.setColor(sf::Color::Black);
    this->csta.setCharacterSize(18);
    this->csta.setPosition(501.0f, 345.0f);

    this->catk.setFont(this->font);
    this->catk.setColor(sf::Color::Black);
    this->catk.setCharacterSize(24);
    this->catk.setPosition(423.0f, 386.0f);

    this->cdef.setFont(this->font);
    this->cdef.setColor(sf::Color::Black);
    this->cdef.setCharacterSize(24);
    this->cdef.setPosition(423.0f, 433.0f);

    this->clif.setFont(this->font);
    this->clif.setColor(sf::Color::Black);
    this->clif.setCharacterSize(24);
    this->clif.setPosition(423.0f, 480.0f);

    return true;
}

void CreaView::render(sf::RenderWindow& window)
{
    if (!this->displaying_crea)
    {
        window.draw(this->sprites[this->BTN_CREA]);
        window.draw(this->sprites[this->BCKG]);
    }
    else
    {
        window.draw(this->sprites[this->BTN_PC]);
        window.draw(this->sprites[this->BCKG_PC]);
    }
    this->draw_content(window);
}

int CreaView::process_event(sf::Event& event, sf::Time elapsed)
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
            if (__X >= 238 && __X <= 393 && __Y >= 10 && __Y <= 90)
            {
                this->displaying_crea = !this->displaying_crea;
            }
            else if (__X >= 464 && __X <= 531 && __Y >= 523 && __Y <= 588)
            {
                if (this->displaying_crea)
                {
                    // send selected crea to pc
                }
                else
                {
                    // send selected crea to the pc
                }
            }
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }

    return new_view;
}

void CreaView::update(sf::RenderWindow& window, sf::Time elapsed)
{

}

void CreaView::draw_content(sf::RenderWindow& window)
{
    if (this->displaying_crea)
    {
        Creature* crea;
        std::string type, statut;

        for (int i=0; i < this->equip->getSize())
        {
            crea = this->equip->getCrea(i);

            switch (crea->getType())
            {
            case Type::NORMAL:
                type = "Normal";
                break;

            case Type::FIRE:
                type = "Feu";
                break;

            case Type::WATER:
                type = "Eau";
                break;

            case Type::GRASS:
                type = "Plante";
                break;

            case Type::FLYING:
                type = "Vol";
                break;

            case Type::FIGHTING:
                type = "Combat";
                break;

            case Type::POISON:
                type = "Poison";
                break;

            case Type::ELECTRIC:
                type = "Electrique";
                break;

            default:
                type = "null";
                break;
            }

            switch (crea->getState())
            {
            case State::BURNED:
                statut = "Brûlé";
                break;

            case State::PARALYSED:
                statut = "Paralysé";
                break;

            case State::POISONED:
                statut = "Empoisonné";
                break;

            case State::STD:
                statut = "Normal";
                break;

            default:
                statut = "null";
                break;
            }

            this->cnam.setString(crea->getName());
            this->clev.setString(to_string<int>(crea->getLevel()));
            this->ctyp.setString(type);
            this->csta.setString(statut);
            this->catk.setString(to_string<int>(crea->getAtk()));
            this->cdef.setString(to_string<int>(crea->getDef()));
            this->clif.setString(to_string<int>(crea->getLife()));

            window.display(this->cnam);
            window.display(this->clev);
            window.display(this->ctyp);
            window.display(this->csta);
            window.display(this->catk);
            window.display(this->cdef);
            window.display(this->clif);
            window.display(this->cimg);
        }
    }
    else
    {
        // ...
    }
}

void CreaView::add_equip(Equip* equip)
{
    this->equip = equip;
}
