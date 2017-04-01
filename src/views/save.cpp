#include <iostream>
#include "../../debug.hpp"

#include "save.hpp"
#include "../constants.hpp"

#define __X event.mouseButton.x
#define __Y event.mouseButton.y

// public
SaveView::SaveView() :
    View(SAVING_VIEW_ID)
    , npc("maleresearcher", "", PNJkind::normal, 264, 530)
    , npcdoing(0)
    , npcway(true)
    , _c(0)
{

}

bool SaveView::load()
{
    sf::Texture bkg;
    if (!bkg.loadFromFile("assets/gui/fd_sauvegarde.png"))
        return false;
    this->textures.add("bkg", bkg);

    this->sprites["BKG"] = sf::Sprite(this->textures.get("bkg"));

    this->sprites["BKG"].setPosition(0.0f, 0.0f);

    if (!this->font.loadFromFile("assets/fonts/pkmnemn.ttf"))
        return false;

    this->text.setFont(this->font);
    this->text.setColor(sf::Color::Black);
    this->text.setCharacterSize(24);
    this->text.setString("Clic pour sauvegarder ta partie !");
    this->text.setPosition((WIN_W - this->text.getGlobalBounds().width) / 2, 275.0f);

    this->npc.load();

    return true;
}

void SaveView::render(sf::RenderWindow& window)
{
    window.draw(this->sprites["BKG"]);
    window.draw(this->text);
    window.draw(this->npc.getCurrentSprite());
}

int SaveView::process_event(sf::Event& event, sf::Time elapsed)
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
            this->text.setString("Sauvegarde en cours de la partie de " + this->defv->getCharacter()->getName());
            this->text.setPosition((WIN_W - this->text.getGlobalBounds().width) / 2, 275.0f);
            this->_c = 1;
            this->save();
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

void SaveView::update(sf::RenderWindow& window, sf::Time elapsed)
{
    if (this->_c > 0)
        this->_c++;

    if (this->last >= sf::seconds(0.05f))
    {
        switch (this->npcdoing)
        {
        case 0:
            if (this->npcway)
                this->npc.simplemove(DIRECTION::up, elapsed);
            else
                this->npc.simplemove(DIRECTION::down, elapsed);
            break;

        case 1:
            if (this->npcway)
                this->npc.simplemove(DIRECTION::left, elapsed);
            else
                this->npc.simplemove(DIRECTION::right, elapsed);
            break;

        case 2:
            if (this->npcway)
                this->npc.simplemove(DIRECTION::right, elapsed);
            else
                this->npc.simplemove(DIRECTION::left, elapsed);
            break;

        case 3:
            if (this->npcway)
                this->npc.simplemove(DIRECTION::up, elapsed);
            else
                this->npc.simplemove(DIRECTION::down, elapsed);
            break;

        default:
            break;
        }

        int n = -1;
        switch (this->npcdoing)
        {
        case 0:
            if (this->npcway && this->npc.getPos().getY() == 290.0f)
                n = 1;
            else if (!this->npcway && this->npc.getPos().getY() == 530.0f)
            {
                this->npcway = true;
            }
            break;

        case 1:
            if (this->npcway && this->npc.getPos().getX() == 0.0f)
                n = 2;
            else if (!this->npcway && this->npc.getPos().getX() == 264.0f)
                n = 0;
            break;

        case 2:
            if (this->npcway && this->npc.getPos().getX() == 516.0f)
                n = 3;
            else if (!this->npcway && this->npc.getPos().getX() == 0.0f)
                n = 1;
            break;

        case 3:
            if (!this->npcway && this->npc.getPos().getY() == 290.0f)
                n = 2;
            else if (this->npcway && this->npc.getPos().getY() == 0.0f)
            {
                this->npcway = false;
            }
            break;

        default:
            break;
        }
        if (n != -1)
        {
            this->npcdoing = n;
        }

        this->last = sf::seconds(0.0f);
    }
    this->last += elapsed;

    this->npc.update(window, elapsed);

    if (this->_c > 500 && this->_c < 900)
    {
        this->text.setString("Sauvegardé !");
        this->text.setPosition((WIN_W - this->text.getGlobalBounds().width) / 2, 275.0f);
    }
    else if (this->_c > 900)
    {
        this->text.setString("Clic pour sauvegarder ta partie !");
        this->text.setPosition((WIN_W - this->text.getGlobalBounds().width) / 2, 275.0f);
        this->_c = 0;
    }
}

void SaveView::setdefv(DefaultView* _defv)
{
    this->defv = _defv;
}

void SaveView::save()
{
    this->defv->getCharacter()->save();

    Json::Value root;
    root["id"] = this->defv->getMap()->getId();

    std::ofstream output("saves/" + this->defv->getCharacter()->getName() + "/map.json");
    output << root;
}
