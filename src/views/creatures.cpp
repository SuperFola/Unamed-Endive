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
    this->text.setFont(this->font);
    this->text.setString("");
    this->text.setCharacterSize(24);
    this->text.setColor(sf::Color::White);
    this->text.setPosition(WIN_W / 2 - this->text.getGlobalBounds().width / 2, 30.0f);

    return true;
}

void CreaView::render(sf::RenderWindow& window)
{
    window.draw(this->sprites[this->BCKG]);
    window.draw(this->text);
    if (!this->displaying_crea)
    {
        window.draw(this->sprites[this->BTN_CREA]);
    }
    else
    {
        window.draw(this->sprites[this->BTN_PC]);
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
            else if (__X >= 0 && __X <= 0 && __Y >= 0 && __Y <= 0)
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
        // ...
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
