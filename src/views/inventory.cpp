#include <iostream>

#include "inventory.hpp"
#include "../abstract/defines.hpp"

#define __X event.mouseButton.x
#define __Y event.mouseButton.y

// public
InventView::InventView() :
    View(INVENTORY_VIEW_ID)
    , current(0)
    , selected(0)
    , offset(0)
    , errdisplay(0)
{

}

bool InventView::load()
{
    // loading textures
    sf::Texture background;
    if (!background.loadFromFile("assets/gui/inventory/fd_inventaire.png"))
        return false;
    this->textures.add("background", background);

    sf::Texture larrow;
    if (!larrow.loadFromFile("assets/gui/inventory/fd_bouton_previous.png"))
        return false;
    this->textures.add("larrow", larrow);

    sf::Texture rarrow;
    if (!rarrow.loadFromFile("assets/gui/inventory/fd_bouton_next.png"))
        return false;
    this->textures.add("rarrow", rarrow);

    sf::Texture p1;
    if (!p1.loadFromFile("assets/inventory/pockets/0.png"))
        return false;
    this->textures.add("pocket1", p1);

    sf::Texture p2;
    if (!p2.loadFromFile("assets/inventory/pockets/1.png"))
        return false;
    this->textures.add("pocket2", p2);

    sf::Texture p3;
    if (!p3.loadFromFile("assets/inventory/pockets/2.png"))
        return false;
    this->textures.add("pocket3", p3);

    sf::Texture p4;
    if (!p4.loadFromFile("assets/inventory/pockets/3.png"))
        return false;
    this->textures.add("pocket4", p4);

    sf::Texture p5;
    if (!p5.loadFromFile("assets/inventory/pockets/4.png"))
        return false;
    this->textures.add("pocket5", p5);

    sf::Texture drop;
    if (!drop.loadFromFile("assets/gui/inventory/fd_bouton_jeter.png"))
        return false;
    this->textures.add("btn_drop", drop);

    sf::Texture vid;
    if (!vid.loadFromFile("assets/gui/inventory/fd_bouton_vider.png"))
        return false;
    this->textures.add("btn_vide", vid);

    sf::Texture use;
    if (!use.loadFromFile("assets/gui/inventory/fd_bouton_utiliser.png"))
        return false;
    this->textures.add("btn_use", use);

    // setting sprites
    this->sprites[this->BCKG] = sf::Sprite(this->textures.get("background"));
    this->sprites[this->LARROW] = sf::Sprite(this->textures.get("larrow"));
    this->sprites[this->RARROW] = sf::Sprite(this->textures.get("rarrow"));
    this->sprites[this->POCKET1] = sf::Sprite(this->textures.get("pocket1"));
    this->sprites[this->POCKET2] = sf::Sprite(this->textures.get("pocket2"));
    this->sprites[this->POCKET3] = sf::Sprite(this->textures.get("pocket3"));
    this->sprites[this->POCKET4] = sf::Sprite(this->textures.get("pocket4"));
    this->sprites[this->POCKET5] = sf::Sprite(this->textures.get("pocket5"));
    this->sprites[this->DROP] = sf::Sprite(this->textures.get("btn_drop"));
    this->sprites[this->VID] = sf::Sprite(this->textures.get("btn_vide"));
    this->sprites[this->USE] = sf::Sprite(this->textures.get("btn_use"));

    // setting pos
    this->sprites[this->BCKG].setPosition(0.0f, 0.0f);
    this->sprites[this->LARROW].setPosition(117.0f, 586.0f);
    this->sprites[this->RARROW].setPosition(498.0f, 586.0f);
    this->sprites[this->POCKET1].setPosition(28.0f, 37.0f);
    this->sprites[this->POCKET2].setPosition(28.0f, 37.0f);
    this->sprites[this->POCKET3].setPosition(28.0f, 37.0f);
    this->sprites[this->POCKET4].setPosition(28.0f, 37.0f);
    this->sprites[this->POCKET5].setPosition(28.0f, 37.0f);
    this->sprites[this->DROP].setPosition(97.0f, 508.0f);
    this->sprites[this->VID].setPosition(165.0f, 508.0f);
    this->sprites[this->USE].setPosition(29.0f, 508.0f);

    // texts
    if (!this->font.loadFromFile(FONTPATH))
        return false;

    setupFont(this->current_pocket_name, this->font, sf::Color::Black, 24)
    this->current_pocket_name.setString("Pocket");
    this->current_pocket_name.setPosition(310.0f - this->current_pocket_name.getLocalBounds().width / 2.0f, 599.0f - this->current_pocket_name.getLocalBounds().height);

    setupFont(this->object_name, this->font, sf::Color::Black, 18)
    this->object_name.setString("object0");
    this->object_name.setPosition(265.0f, 37.0f);

    setupFont(this->object_desc, this->font, sf::Color::Black, 18)
    this->object_desc.setString("");
    this->object_desc.setPosition(30.0f, 300.0f);

    setupFont(this->errmsg, this->font, sf::Color::Red, 24)
    this->errmsg.setPosition(0.0f, 6.0f);

    return true;
}

void InventView::render(sf::RenderWindow& window)
{
    window.draw(this->sprites[this->BCKG]);
    window.draw(this->current_pocket_name);
    window.draw(this->sprites[this->LARROW]);
    window.draw(this->sprites[this->RARROW]);
    window.draw(this->sprites[this->DROP]);
    window.draw(this->sprites[this->VID]);
    window.draw(this->sprites[this->USE]);
    this->draw_content(window);
    if (this->errdisplay > 0)
        window.draw(this->errmsg);
}

int InventView::process_event(sf::Event& event, sf::Time elapsed)
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

        case sf::Keyboard::Left:
            this->current--;
            this->change_pocket();
            this->current_pocket_name.setString(this->bag->getPocket(this->current)->getName());
            break;

        case sf::Keyboard::Right:
            this->current++;
            this->change_pocket();
            this->current_pocket_name.setString(this->bag->getPocket(this->current)->getName());
            break;

        default:
            break;
        }
        break;

    case sf::Event::MouseButtonPressed:
        switch(event.mouseButton.button)
        {
        case sf::Mouse::Button::Left:
            if (__X >= 115 && __X <= 135 && __Y >= 589 && __Y <= 609)
            {
                // previous
                this->current--;
                this->change_pocket();
                this->current_pocket_name.setString(this->bag->getPocket(this->current)->getName());
            }
            else if (__X >= 500 && __X <= 520 && __Y >= 589 && __Y <= 609)
            {
                // next
                this->current++;
                this->change_pocket();
                this->current_pocket_name.setString(this->bag->getPocket(this->current)->getName());
            }
            else if (__X >= 97 && __X <= 149 && __Y >= 508 && __Y <= 560)
            {
                // drop
                this->bag->getPocket(this->current)->drop_object(this->selected + this->offset);
            }
            else if (__X >= 165 && __X <= 217 && __Y >= 508 && __Y <= 560)
            {
                // dropping all
                this->bag->getPocket(this->current)->dropall_object(this->selected + this->offset);
            }
            else if (__X >= 29 && __X <= 81 && __Y >= 508 && __Y <= 560)
            {
                // use
                if ((ObjectsTable::getType(this->bag->getPocket(this->current)->getObject(this->selected + this->offset)) != ObjType::player && OMessenger::getLock() == this->getId()) ||
                    OMessenger::getLock() != this->getId())
                {
                    ObjUDD udd = this->bag->getPocket(this->current)->useObject(this->selected + this->offset);
                    if (udd.target_view != UNREACHABLE_VIEW_ID)
                    {
                        // we can see what to do with that now
                        OMessenger::set(udd);
                    }
                    else
                    {
                        // display error message
                        this->errdisplay = 1;
                        this->errmsg.setString("Impossible d'utiliser l'objet");
                        this->errmsg.setPosition(WIN_W / 2.0f - this->errmsg.getGlobalBounds().width / 2.0f, this->errmsg.getPosition().y);
                    }
                }
                else
                {
                    this->errdisplay = 1;
                    this->errmsg.setString("Impossible d'utiliser l'objet, vous êtes en combat");
                    this->errmsg.setPosition(WIN_W / 2.0f - this->errmsg.getGlobalBounds().width / 2.0f, this->errmsg.getPosition().y);
                }
            }
            else if (__X >= 258 && __X <= 619 && __Y >= 30 && __Y <= 554)
            {
                // clic in the objects list, need to find which one was picked
                int r = (__Y - 37) / (this->object_name.getCharacterSize() + 4);

                if (0 <= r + this->offset && r + this->offset <= bag->getPocket(this->current)->getSize())
                    this->selected = r;
            }
            break;

        default:
            break;
        }
        break;

    case sf::Event::MouseWheelScrolled:
        this->offset -= event.mouseWheelScroll.delta;
        if (this->offset <= -1)
            this->offset = 0;
        else if (this->offset >= this->bag->getPocket(this->current)->getSize())
            this->offset = (this->bag->getPocket(this->current)->getSize() != 0) ? (this->bag->getPocket(this->current)->getSize() - 1): 0;
        break;

    default:
        break;
    }

    return new_view;
}

void InventView::update(sf::RenderWindow& window, sf::Time elapsed)
{
    if (this->errdisplay > 0)
        this->errdisplay++;
    if (!(this->errdisplay % 640))
        this->errdisplay = 0;
}

void InventView::change_pocket()
{
    // checking if the current pocket is correct
    if (this->current < 0)
        this->current = 4;
    else if (this->current > 4)
        this->current = 0;

    // reset current offset
    this->offset = 0;

    this->selected = 0;
}

void InventView::draw_content(sf::RenderWindow& window)
{
    switch (this->current)
    {
    case 0:
        window.draw(this->sprites[this->POCKET1]);
        break;

    case 1:
        window.draw(this->sprites[this->POCKET2]);
        break;

    case 2:
        window.draw(this->sprites[this->POCKET3]);
        break;

    case 3:
        window.draw(this->sprites[this->POCKET4]);
        break;

    case 4:
        window.draw(this->sprites[this->POCKET5]);
        break;
    }

    std::string name;
    for (int i=this->offset; i < this->bag->getPocket(this->current)->getSize(); i++)
    {
        // object name
        Object* obj = this->bag->getPocket(this->current)->getObject(i);
        name = ObjectsTable::getName(obj) + " (" + to_string(obj->getQuantity()) + ")";
        if (i == this->selected + this->offset)
            name = "> " + name;
        this->object_name.setString(name);

        // description is selected (regarding to the index i)
        if (i == this->selected + this->offset)
        {
            this->object_desc.setString(wrapText(sf::String(ObjectsTable::getDesc(obj)), 200, this->font, this->object_desc.getCharacterSize()));
            window.draw(this->object_desc);
        }

        // drawing
        window.draw(this->object_name);

        // change the position for the next object name
        this->object_name.setPosition(this->object_name.getPosition().x, this->object_name.getPosition().y + this->object_name.getCharacterSize() + 4.0f);

        // stop if we have displayed max items
        if (i == this->offset + 22)
            break;
    }
    this->object_name.setPosition(this->object_name.getPosition().x, 37.0f);
}

void InventView::add_bag(Bag* bag)
{
    this->bag = bag;
    this->current_pocket_name.setString(this->bag->getPocket(this->current)->getName());
}

InventView::~InventView()
{
    this->bag = NULL;
}
