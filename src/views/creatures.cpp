#include <iostream>

#include "creatures.hpp"
#include "../constants.hpp"

#define __X event.mouseButton.x
#define __Y event.mouseButton.y

// public
CreaView::CreaView() :
    View(MYCREATURES_VIEW_ID)
    , displaying_crea(true)
    , selected(0)
    , offset(0)
    , err_duration(0.0f)
{

}

bool CreaView::load()
{
    // loading textures
    sf::Texture background;
    if (!background.loadFromFile("assets/gui/equip/fd_equip.png"))
        return false;
    this->textures.add("background", background);

    sf::Texture bckg_pc;
    if (!bckg_pc.loadFromFile("assets/gui/equip/fd_pc.png"))
        return false;
    this->textures.add("background_pc", bckg_pc);

    sf::Texture btn_pc;
    if (!btn_pc.loadFromFile("assets/gui/equip/pc.png"))
        return false;
    this->textures.add("btn_pc", btn_pc);

    sf::Texture btn_crea;
    if (!btn_crea.loadFromFile("assets/gui/equip/team.png"))
        return false;
    this->textures.add("btn_crea", btn_crea);

    // setting sprites
    this->sprites[this->BCKG] = sf::Sprite(this->textures.get("background"));
    this->sprites[this->BCKG_PC] = sf::Sprite(this->textures.get("background_pc"));
    this->sprites[this->BTN_PC] = sf::Sprite(this->textures.get("btn_pc"));
    this->sprites[this->BTN_CREA] = sf::Sprite(this->textures.get("btn_crea"));

    // setting pos
    this->sprites[this->BCKG].setPosition(0.0f, 0.0f);
    this->sprites[this->BCKG_PC].setPosition(0.0f, 0.0f);
    this->sprites[this->BTN_PC].setPosition(238.0f, 10.0f);
    this->sprites[this->BTN_CREA].setPosition(238.0f, 10.0f);

    // texts
    if (!this->font.loadFromFile("assets/fonts/pkmnemn.ttf"))
        return false;

    this->cnam.setFont(this->font);
    this->cnam.setFillColor(sf::Color::Black);
    this->cnam.setCharacterSize(18);
    this->cnam.setPosition(388.0f, 155.0f);

    this->clev.setFont(this->font);
    this->clev.setFillColor(sf::Color::Black);
    this->clev.setCharacterSize(24);
    this->clev.setPosition(565.0f, 150.0f);

    this->ctyp.setFont(this->font);
    this->ctyp.setFillColor(sf::Color::Black);
    this->ctyp.setCharacterSize(18);
    this->ctyp.setPosition(428.0f, 342.0f);

    this->csta.setFont(this->font);
    this->csta.setFillColor(sf::Color::Black);
    this->csta.setCharacterSize(18);
    this->csta.setPosition(505.0f, 342.0f);

    this->catk.setFont(this->font);
    this->catk.setFillColor(sf::Color::Black);
    this->catk.setCharacterSize(24);
    this->catk.setPosition(423.0f, 380.0f);

    this->cdef.setFont(this->font);
    this->cdef.setFillColor(sf::Color::Black);
    this->cdef.setCharacterSize(24);
    this->cdef.setPosition(423.0f, 427.0f);

    this->clif.setFont(this->font);
    this->clif.setFillColor(sf::Color::Black);
    this->clif.setCharacterSize(24);
    this->clif.setPosition(423.0f, 474.0f);

    this->cimg = sf::Sprite();
    this->cimg.setPosition(416.0f, 194.0f);

    this->lsdata.setFont(this->font);
    this->lsdata.setFillColor(sf::Color::White);
    this->lsdata.setCharacterSize(20);

    this->error_msg.setFont(this->font);
    this->error_msg.setFillColor(sf::Color::Red);
    this->error_msg.setCharacterSize(26);
    this->error_msg.setPosition(0.0f, 106.0f);

    return true;
}

void CreaView::post_load()
{
    this->set_cimg(0);
}

void CreaView::render(sf::RenderWindow& window)
{
    if (this->displaying_crea)
    {
        window.draw(this->sprites[this->BCKG]);
        window.draw(this->sprites[this->BTN_CREA]);
    }
    else
    {
        window.draw(this->sprites[this->BCKG_PC]);
        window.draw(this->sprites[this->BTN_PC]);
    }
    this->draw_content(window);
    if (this->err_duration != 0.0f)
        window.draw(this->error_msg);
}

void CreaView::set_cimg(int ry)
{
    int container_sz = (this->displaying_crea) ? this->equip->getSize() : this->pc->getSize();

    if (container_sz)
    {
        // resetRect= true to avoid cropped images
        this->cimg.setTexture((this->displaying_crea) ? this->creaload->get(this->dex->getInfo(this->equip->getCrea(ry)->getId()).file) : this->creaload->get(this->dex->getInfo(this->pc->getCrea(ry)->getId()).file), true);

        sf::Vector2u sz = this->cimg.getTexture()->getSize();
        float max_sz = (sz.x > sz.y) ? sz.x : sz.y;
        float sc = (max_sz == sz.x) ? (159.0f / max_sz) : (141.0f / max_sz);

        this->cimg.setScale(sc, sc);

        sf::Vector2u nsz = this->cimg.getTexture()->getSize();
        this->cimg.setPosition(416.0f + (160.0f - nsz.x * sc) / 2.0f, 194.0f + (142.0f - nsz.y * sc) / 2.0f);
    }
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
                // changing the "view" (pc/equip)
                this->displaying_crea = !this->displaying_crea;
                this->offset = 0;
                this->selected = 0;
                this->set_cimg(0);
            }
            else if (__X >= 464 && __X <= 531 && __Y >= 523 && __Y <= 588)
            {
                // transfering creature
                this->send_to(this->selected);
                this->selected -= 1;  // to avoid problem and always have a creature selected
                if (this->selected == -1)
                    this->selected = 0;
                this->set_cimg(this->selected);
            }
            else if (__X >= 18 && __X <= 234 && __Y >= 144 && __Y <= 601)
            {
                // selecting a creature
                int lim = (this->displaying_crea) ? 0 : this->offset;
                int ry = (__Y - 144) / 79;
                if (!this->displaying_crea)
                    ry += this->offset;
                int sz = (this->displaying_crea) ? this->equip->getSize() : this->pc->getSize();

                if (lim <= ry && ry < sz)
                {
                    // we are in a correct range
                    this->selected = ry;
                    this->set_cimg(ry);
                }
            }
            break;

        default:
            break;
        }
        break;

    case sf::Event::MouseWheelScrolled:
        if (!this->displaying_crea)
        {
            this->offset -= event.mouseWheelScroll.delta;
            if (this->offset <= -1)
                this->offset = 0;
            else if (this->offset >= this->pc->getSize())
                this->offset = (this->pc->getSize() != 0) ? (this->pc->getSize() - 1) : 0;
            this->selected = this->offset;
            this->set_cimg(this->selected);
        }
        break;

    default:
        break;
    }

    return new_view;
}

void CreaView::update(sf::RenderWindow& window, sf::Time elapsed)
{
    this->err_duration -= elapsed.asSeconds();
    if (this->err_duration < 0.0f)
        this->err_duration = 0.0f;

    if (OMessenger::get().target_view == this->getId())
    {
        switch (OMessenger::get().type)
        {
        case ObjType::healpv:
            break;

        case ObjType::healpp:
            break;

        case ObjType::healstatus:
            break;

        case ObjType::levelup:
            break;

        case ObjType::lowercooldown:
            break;

        default:
            // should never land here
            DebugLog(SH_WARN, "We land here but we should'nt. What type of object is it ? => " << OMessenger::get().type);
            break;
        }
        OMessenger::empty();
    }
}

void CreaView::draw_content(sf::RenderWindow& window)
{
    this->lsdata.setPosition(30.0f, 150.0f);

    Creature* crea;
    std::string type, statut;
    int i, sz;

    i = (this->displaying_crea) ? 0 : this->offset;
    sz = (this->displaying_crea) ? this->equip->getSize() : ((this->offset + 6 < this->pc->getSize()) ? this->offset + 6 : this->pc->getSize());

    for (i; i < sz; i++)
    {
        if (this->displaying_crea)
            crea = this->equip->getCrea(i);
        else
            crea = this->pc->getCrea(i);

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

        if (this->selected == i)
        {
            this->cnam.setString(crea->getName());
            this->clev.setString(to_string<int>(crea->getLevel()));
            this->ctyp.setString(type);
            this->csta.setString(statut);
            this->catk.setString(to_string<int>(crea->getAtk()));
            this->cdef.setString(to_string<int>(crea->getDef()));
            this->clif.setString(to_string<int>(crea->getLife()));

            window.draw(this->cnam);
            window.draw(this->clev);
            window.draw(this->ctyp);
            window.draw(this->csta);
            window.draw(this->catk);
            window.draw(this->cdef);
            window.draw(this->clif);
            window.draw(this->cimg);

            this->lsdata.setFillColor(sf::Color::Green);
        }
        else
            this->lsdata.setFillColor(sf::Color::White);

        this->lsdata.setString(crea->getName() + "\n\n" + type + ", niveau: " + to_string<int>(crea->getLevel()));
        window.draw(this->lsdata);

        this->lsdata.setPosition(this->lsdata.getPosition().x, this->lsdata.getPosition().y + 79.0f);
    }
}

void CreaView::add_equip(Equip* equip)
{
    this->equip = equip;
}

void CreaView::add_pc(Equip* pc)
{
    this->pc = pc;
}

void CreaView::send_to(int id)
{
    // if this->displaying_crea == true, then send to pc
    // otherwise send to equip

    if (this->displaying_crea)
    {
        // send selected crea to pc
        if (this->equip->getSize() > 1)
        {
            if (this->pc->add_creature(this->equip->getCrea(this->selected)))
            {
                // it succeeded, we can delete the creature from the old container
                this->equip->remove_creature(this->selected);
            }
            else
            {
                // print error message
                this->error_msg.setString("Impossible d'envoyer la créature au PC");
                this->err_duration = 7.0f;
                this->error_msg.setPosition(WIN_W / 2 - this->error_msg.getGlobalBounds().width / 2, this->error_msg.getPosition().y);
            }
        }
        else
        {
            // print error message
            this->error_msg.setString("Votre équipe ne peut pas être vide !");
            this->err_duration = 7.0f;
            this->error_msg.setPosition(WIN_W / 2 - this->error_msg.getGlobalBounds().width / 2, this->error_msg.getPosition().y);
        }
    }
    else
    {
        // send selected crea to the pc
        if (this->equip->add_creature(this->pc->getCrea(this->selected)))
        {
            // it succeeded, delete the creature from the old container as before
            this->pc->remove_creature(this->selected);
        }
        else
        {
            // display a nice error message
            this->error_msg.setString("Impossible d'envoyer la créature dans l'équipe");
            this->err_duration = 7.0f;
            this->error_msg.setPosition(WIN_W / 2 - this->error_msg.getGlobalBounds().width / 2, this->error_msg.getPosition().y);
        }
    }
}

void CreaView::add_creaload(CreaturesLoader* creaload)
{
    this->creaload = creaload;
}

void CreaView::add_dex(Dex* dex)
{
    this->dex = dex;
}
