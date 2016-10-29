#include <SFML/Graphics.hpp>
#include <iostream>

#include "menu.hpp"
#include "../../constants.hpp"

// public
MenuHUD::MenuHUD() :
    Hud(MENU_VIEW_ID)
    , current(-1)
{
    if (!this->font.loadFromFile("assets/fonts/pkmnemn.ttf"))
        std::cout << "Can not load font at assets/fonts/pkmnemn.ttf" << std::endl;
}

bool MenuHUD::load()
{
    // loading textures
    sf::Texture* bg;
    if (!bg->loadFromFile("assets/gui/fd_menu.png"))
        return false;
    this->textures.push_back(bg);

    sf::Texture* cat;
    if (!cat->loadFromFile("assets/gui/fd_categorie_menu.png"))
        return false;
    this->textures.push_back(cat);

    sf::Texture* cat_sel;
    if (!cat_sel->loadFromFile("assets/gui/fd_categorie_selected_menu.png"))
        return false;
    this->textures.push_back(cat_sel);

    // creating sprites
    this->sprites.push_back(sf::Sprite(this->textures[this->BACKGROUND]));
    this->sprites.push_back(sf::Sprite(this->textures[this->BG_CATEGORY]));
    this->sprites.push_back(sf::Sprite(this->textures[this->BG_CATEGORY_SELECTED]));

    // creating texts
    sf::Text* crea(L"Créatures", this->font, 24);
    crea->setColor(sf::Color::Black);
    this->texts.push_back(crea);

    sf::Text* invent("Inventaire", this->font, 24);
    invent->setColor(sf::Color::Black);
    this->texts.push_back(invent);

    sf::Text* carte("Carte", this->font, 24);
    carte->setColor(sf::Color::Black);
    this->texts.push_back(carte);

    sf::Text* save("Sauvegarder", this->font, 24);
    save->setColor(sf::Color::Black);
    this->texts.push_back(save);

    sf::Text* dex("Indexeur", this->font, 24);
    dex->setColor(sf::Color::Black);
    this->texts.push_back(dex);

    sf::Text* goback("Retour", this->font, 24);
    goback->setColor(sf::Color::Black);
    this->texts.push_back(goback);

    return true;
}

void MenuHUD::render(sf::RenderWindow& window)
{
    if (!this->is_trigered)
        goto dont;

    window.draw(*(this->sprites[this->BACKGROUND]));
    window.draw(*(this->sprites[this->BG_CATEGORY]));
    window.draw(*(this->sprites[this->BG_CATEGORY_SELECTED]));
    window.draw(*(this->texts[this->TXT_CREA]));
    window.draw(*(this->texts[this->TXT_INVENT]));
    window.draw(*(this->texts[this->TXT_MAP]));
    window.draw(*(this->texts[this->TXT_SAVE]));
    window.draw(*(this->texts[this->TXT_DEX]));
    window.draw(*(this->texts[this->TXT_BACK]));

    dont:;
}

void MenuHUD::update(sf::RenderWindow& window, sf::Time elapsed)
{
    if (!this->is_trigered)
        goto dont2;
    sf::Vector2i mouse = sf::Mouse::getPosition(window);

    dont2:;
}

int MenuHUD::process_event(sf::Event& event, sf::Time elapsed)
{
    int new_view = -1;

    if (!this->is_trigered)
        goto dont3;

    switch(event.type)
    {
    case sf::Event::KeyPressed:
        switch(event.key.code)
        {
        case sf::Keyboard::Return:
            if (this->current != -1)
                new_view = this->current;
            break;

        case sf::Keyboard::Escape:
            this->is_trigered = false;
            goto dont3;
            break;

        default:
            break;
        }
        break;

    case sf::Event::MouseButtonPressed:
        switch(event.mouseButton.button)
        {
        case sf::Mouse::Button::Left:
            this->clic(event.mouseButton.x, event.mouseButton.y);
            if (this->current != -1)
                new_view = this->current;
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }

    dont3:
    // return the new view id, if we need to change it
    return new_view;
}

// private
void MenuHUD::clic(int x, int y)
{

}
