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

    // setting pos
    this->sprites[this->BACKGROUND].setPosition(20.0f, 50.0f);

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

    // setting pos (texts)
    this->texts[this->TXT_CREA].setPosition(54.0f, 76.0f);
    this->texts[this->TXT_INVENT].setPosition(349.0f, 76.0f);
    this->texts[this->TXT_MAP].setPosition(54.0f, 218.0f);
    this->texts[this->TXT_SAVE].setPosition(349.0f, 218.0f);
    this->texts[this->TXT_DEX].setPosition(54.0f, 360.0f);
    this->texts[this->TXT_BACK].setPosition(349.0f, 360.0f);

    return true;
}

void MenuHUD::render(sf::RenderWindow& window)
{
    if (!this->isTriggered())
        goto dont;

    window.draw(this->sprites[this->BACKGROUND]);

    // drawing cases
    {
        float y = 61.0f;
        for (int i=0; i < 6; i++)
        {
            bool left = !(i % 2);
            float x;
            if (left) x = 30.0f;
            else x = 325.0f;

            if (i != this->current)
            {
                this->sprites[this->BG_CATEGORY].setPosition(x, y);
                window.draw(this->sprites[this->BG_CATEGORY]);
            }
            else
            {
                this->sprites[this->BG_CATEGORY_SELECTED].setPosition(x, y);
                window.draw(this->sprites[this->BG_CATEGORY_SELECTED]);
            }

            if (!left)
                y += 143.0f;
        }
    }

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

        case sf::Keyboard::RShift:
            this->current = -1;
            this->setTrigger(false);
            goto dont3;
            break;

        case sf::Keyboard::Up:
            this->current -= 2;
            if (this->current == -2)
                this->current = 4;
            else if (this->current == -1)
                this->current = 5;
            break;

        case sf::Keyboard::Down:
            this->current += 2;
            if (this->current == 6)
                this->current = 0;
            else if (this->current == 7)
                this->current = 1;
            break;

        case sf::Keyboard::Left:
            this->current -= 1;
            if (this->current == -1)
                this->current = 5;
            break;

        case sf::Keyboard::Right:
            this->current += 1;
            if (this->current == 6)
                this->current = 0;
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
    if (new_view == 5) // back button
    {
        this->setTrigger(false);
        return -1;
    }
    return this->id_to_vid(new_view);
}

// private
void MenuHUD::clic(int x, int y)
{
    bool left = (x >= 30) && (x <= 320);
    int nb = (y - 61) / 143;

    switch (nb)
    {
    case 0:
        if (!left) this->current = 1;
        else this->current = 0;
        break;

    case 1:
        if (!left) this->current = 3;
        else this->current = 2;
        break;

    case 2:
        if (!left) this->current = 5;
        else this->current = 4;
        break;
    }
}

int MenuHUD::id_to_vid(int value)
{
    int ret;
    switch (value)
    {
    case -1:
        ret = -1;
        break;

    case 0:
        ret = MYCREATURES_VIEW_ID;
        break;

    case 1:
        ret = INVENTORY_VIEW_ID;
        break;

    case 2:
        ret = MAP_VIEW_ID;
        break;

    case 3:
        ret = SAVING_VIEW_ID;
        break;

    case 4:
        ret = DEX_VIEW_ID;
        break;

    case 5:
        ret = DEFAULT_VIEW_ID;
        break;

    default:
        ret = -1;  // should not happen
        break;
    }

    return ret;
}
