#include <iostream>

#include "dex.hpp"
#include "../constants.hpp"

#define __X event.mouseButton.x
#define __Y event.mouseButton.y

// public
DexView::DexView() :
    View(DEX_VIEW_ID)
    , displaying_crea(true)
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
        // ...
    }
    else
    {
        // ...
    }
}
