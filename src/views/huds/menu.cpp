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
    std::cout << "menu hud loaded" << std::endl;
}

bool MenuHUD::load()
{
    // loading textures
    sf::Texture bg;
    if (!bg.loadFromFile("assets/gui/fd_menu.png"))
        return false;
    this->textures.addTexture(this->BACKGROUND, bg);

    sf::Texture cat;
    if (!cat.loadFromFile("assets/gui/fd_categorie_menu.png"))
        return false;
    this->textures.addTexture(this->BG_CATEGORY, cat);

    sf::Texture cat_sel;
    if (!cat_sel.loadFromFile("assets/gui/fd_categorie_selected_menu.png"))
        return false;
    this->textures.addTexture(this->BG_CATEGORY_SELECTED, cat_sel);

    // creating sprites
    this->sprites[this->BACKGROUND] = sf::Sprite(this->textures.getTexture(this->BACKGROUND));
    this->sprites[this->BG_CATEGORY] = sf::Sprite(this->textures.getTexture(this->BG_CATEGORY));
    this->sprites[this->BG_CATEGORY_SELECTED] = sf::Sprite(this->textures.getTexture(this->BG_CATEGORY_SELECTED));

    // creating texts
    sf::Text crea;
    crea.setFont(this->font);
    crea.setString("Créatures");
    crea.setCharacterSize(24);
    crea.setColor(sf::Color::Black);
    this->texts[this->TXT_CREA] = crea;

    sf::Text invent;
    invent.setFont(this->font);
    invent.setString("Inventaire");
    invent.setCharacterSize(24);
    invent.setColor(sf::Color::Black);
    this->texts[this->TXT_INVENT] = invent;

    sf::Text carte;
    carte.setFont(this->font);
    carte.setString("Carte");
    carte.setCharacterSize(24);
    carte.setColor(sf::Color::Black);
    this->texts[this->TXT_MAP] = carte;

    sf::Text save;
    save.setFont(this->font);
    save.setString("Sauvegarder");
    save.setCharacterSize(24);
    save.setColor(sf::Color::Black);
    this->texts[this->TXT_SAVE] = save;

    sf::Text dex;
    dex.setFont(this->font);
    dex.setString("Indexeur");
    dex.setCharacterSize(24);
    dex.setColor(sf::Color::Black);
    this->texts[this->TXT_DEX] = dex;

    sf::Text goback;
    goback.setFont(this->font);
    goback.setString("Retour");
    goback.setCharacterSize(24);
    goback.setColor(sf::Color::Black);
    this->texts[this->TXT_BACK] = goback;

    return true;
}

void MenuHUD::render(sf::RenderWindow& window)
{
    if (!this->isTriggered())
        goto dont;

    window.draw(this->sprites[this->BACKGROUND]);
    window.draw(this->sprites[this->BG_CATEGORY]);
    window.draw(this->sprites[this->BG_CATEGORY_SELECTED]);
    window.draw(this->texts[this->TXT_CREA]);
    window.draw(this->texts[this->TXT_INVENT]);
    window.draw(this->texts[this->TXT_MAP]);
    window.draw(this->texts[this->TXT_SAVE]);
    window.draw(this->texts[this->TXT_DEX]);
    window.draw(this->texts[this->TXT_BACK]);

    dont:;
}

void MenuHUD::update(sf::RenderWindow& window, sf::Time elapsed)
{
    if (this->isTriggered())
        sf::Vector2i mouse = sf::Mouse::getPosition(window);
}

int MenuHUD::process_event(sf::Event& event, sf::Time elapsed)
{
    int new_view = -1;

    if (!this->isTriggered())
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
            this->setTrigger(false);
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
