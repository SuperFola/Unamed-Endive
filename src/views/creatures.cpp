#include <iostream>

#include "creatures.hpp"
#include "../constants.hpp"

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
    if (!background.loadFromFile("assets/gui/fd_creatures.png"))
        return false;
    this->textures.addTexture("background", background);

    sf::Texture btn_pc;
    if (!btn_pc.loadFromFile("assets/gui/fd_bouton_pc.png"))
        return false;
    this->textures.addTexture("btn_pc", btn_pc);

    sf::Texture btn_to_pc;
    if (!btn_to_pc.loadFromFile("assets/gui/fd_bouton_to_pc.png"))
        return false;
    this->textures.addTexture("btn_to_pc", btn_to_pc);

    sf::Texture btn_crea;
    if (!btn_crea.loadFromFile("assets/gui/fd_bouton_poche_creatures.png"))
        return false;
    this->textures.addTexture("btn_crea", btn_crea);

    sf::Texture btn_to_crea;
    if (!btn_to_crea.loadFromFile("assets/gui/fd_bouton_to_poche_creatures.png"))
        return false;
    this->textures.addTexture("btn_to_crea", btn_to_crea);

    // setting sprites
    this->sprites[this->BCKG] = sf::Sprite(this->textures.getTexture("background"));
    this->sprites[this->BTN_PC] = sf::Sprite(this->textures.getTexture("btn_pc"));
    this->sprites[this->BTN_TO_PC] = sf::Sprite(this->textures.getTexture("btn_to_pc"));
    this->sprites[this->BTN_CREA] = sf::Sprite(this->textures.getTexture("btn_crea"));
    this->sprites[this->BTN_TO_CREA] = sf::Sprite(this->textures.getTexture("btn_to_crea"));

    // setting pos
    this->sprites[this->BCKG].setPosition(20.0f, 20.0f);
    this->sprites[this->BTN_PC].setPosition(587.0f, 30.0f);
    this->sprites[this->BTN_TO_PC].setPosition(507.0f, 30.0f);
    this->sprites[this->BTN_CREA].setPosition(587.0f, 30.0f);
    this->sprites[this->BTN_TO_CREA].setPosition(507.0f, 30.0f);

    // texts
    if (!this->font.loadFromFile("assets/fonts/pkmnemn.ttf"))
        return false;
    this->text.setFont(this->font);
    this->text.setString("Equipe");
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
        window.draw(this->sprites[this->BTN_TO_CREA]);
    }
    else
    {
        window.draw(this->sprites[this->BTN_PC]);
        window.draw(this->sprites[this->BTN_TO_PC]);
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
        default:
            break;
        }
        break;

    case sf::Event::MouseButtonPressed:
        switch(event.mouseButton.button)
        {
        case sf::Mouse::Button::Left:
            if (event.mouseButton.x >= 587 && event.mouseButton.x <= 668 && event.mouseButton.y >= 30 && event.mouseButton.x <= 53)
            {
                this->displaying_crea = !this->displaying_crea;
                if (this->displaying_crea)
                    this->text.setString("Equipe");
                else
                    this->text.setString("PC");
            }
            else if (event.mouseButton.x >= 507 && event.mouseButton.x <= 577 && event.mouseButton.y >= 30 && event.mouseButton.x <= 53)
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
