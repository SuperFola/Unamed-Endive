#include <iostream>

#include "fight.hpp"
#include "../constants.hpp"

#define __X event.mouseButton.x
#define __Y event.mouseButton.y

FightView::FightView() :
    View(FIGHT_VIEW_ID)
    , env(FightEnv::Forest)
{

}

bool FightView::load()
{
    sf::Texture bk1;
    if (!bk1.loadFromFile("assets/gui/fight/background1.png"))
        return false;
    this->textures.addTexture("bk1", bk1);

    sf::Texture bk2;
    if (!bk2.loadFromFile("assets/gui/fight/background2.png"))
        return false;
    this->textures.addTexture("bk2", bk2);

    sf::Texture bk3;
    if (!bk3.loadFromFile("assets/gui/fight/background3.png"))
        return false;
    this->textures.addTexture("bk3", bk3);

    sf::Texture gr1;
    if (!gr1.loadFromFile("assets/gui/fight/ground1.png"))
        return false;
    this->textures.addTexture("gr1", gr1);

    sf::Texture gr2;
    if (!gr2.loadFromFile("assets/gui/fight/ground2.png"))
        return false;
    this->textures.addTexture("gr2", gr2);

    sf::Texture gr3;
    if (!gr3.loadFromFile("assets/gui/fight/ground3.png"))
        return false;
    this->textures.addTexture("gr3", gr3);

    sf::Texture tool;
    if (!tool.loadFromFile("assets/gui/fight/bottom.png"))
        return false;
    this->textures.addTexture("tool", tool);

    sf::Texture overlay;
    if (!overlay.loadFromFile("assets/gui/fight/overlay.png"))
        return false;
    this->textures.addTexture("overlay", overlay);

    sf::Texture lifebar;
    if (!lifebar.loadFromFile("assets/gui/fight/lifebar.png"))
        return false;
    this->textures.addTexture("lifebar", lifebar);

    this->sprites[this->BKG1] = sf::Sprite(this->textures.getTexture("bk1"));
    this->sprites[this->BKG2] = sf::Sprite(this->textures.getTexture("bk2"));
    this->sprites[this->BKG3] = sf::Sprite(this->textures.getTexture("bk3"));
    this->sprites[this->GRD1] = sf::Sprite(this->textures.getTexture("gr1"));
    this->sprites[this->GRD2] = sf::Sprite(this->textures.getTexture("gr2"));
    this->sprites[this->GRD3] = sf::Sprite(this->textures.getTexture("gr3"));
    this->sprites[this->TOOLS] = sf::Sprite(this->textures.getTexture("tool"));
    this->sprites[this->OVERLAY] = sf::Sprite(this->textures.getTexture("overlay"));
    this->sprites[this->LIFEBAR] = sf::Sprite(this->textures.getTexture("lifebar"));

    this->sprites[this->BKG1].setPosition(0.0f, 0.0f);
    this->sprites[this->BKG2].setPosition(0.0f, 0.0f);
    this->sprites[this->BKG3].setPosition(0.0f, 0.0f);
    this->sprites[this->OVERLAY].setPosition(0.0f, 0.0f);
    this->sprites[this->TOOLS].setPosition(0.0f, WIN_H - 150.0f);

    if (!this->font.loadFromFile("assets/fonts/pkmnemn.ttf"))
        return false;

    this->action.setFont(this->font);
    this->action.setFillColor(sf::Color::Black);
    this->action.setCharacterSize(24);
    this->action.setPosition(5.0f, WIN_H - 155.0f);
    this->action.setString("Ceci est un message d'action");

    this->ennemy.setFont(this->font);
    this->ennemy.setFillColor(sf::Color::Black);
    this->ennemy.setCharacterSize(24);

    this->me.setFont(this->font);
    this->me.setFillColor(sf::Color::Black);
    this->me.setCharacterSize(24);

    return true;
}

void FightView::render(sf::RenderWindow& window)
{
    switch (this->env)
    {
    case FightEnv::CityWithForest:
        window.draw(this->sprites[this->BKG1]);
        this->sprites[this->GRD1].setPosition(289.0f, 166.0f);
        window.draw(this->sprites[this->GRD1]);
        this->sprites[this->GRD1].setPosition(17.0f, 418.0f);
        window.draw(this->sprites[this->GRD1]);
        break;

    case FightEnv::Forest:
        window.draw(this->sprites[this->BKG2]);
        this->sprites[this->GRD2].setPosition(289.0f, 166.0f);
        window.draw(this->sprites[this->GRD2]);
        this->sprites[this->GRD2].setPosition(17.0f, 418.0f);
        window.draw(this->sprites[this->GRD2]);
        break;

    case FightEnv::Sea:
        window.draw(this->sprites[this->BKG3]);
        this->sprites[this->GRD3].setPosition(289.0f, 166.0f);
        window.draw(this->sprites[this->GRD3]);
        this->sprites[this->GRD3].setPosition(17.0f, 418.0f);
        window.draw(this->sprites[this->GRD3]);
        break;

    default:
        window.clear(sf::Color::Blue);
        break;
    }
    window.draw(this->sprites[this->OVERLAY]);
    window.draw(this->sprites[this->TOOLS]);
    window.draw(this->action);
    window.draw(this->ennemy);
    window.draw(this->me);
}

int FightView::process_event(sf::Event& event, sf::Time elapsed)
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

void FightView::update(sf::RenderWindow& window, sf::Time elapsed)
{

}

void FightView::set_env(FightEnv new_env)
{
    this->env = new_env;
}
