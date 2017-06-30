#include <iostream>
#include <random>
#include <ctime>
#include <cstdlib>
#include "../../debug.hpp"

#include "fight.hpp"
#include "../constants.hpp"

FightView::FightView() :
    View(FIGHT_VIEW_ID)
    , env(FightEnv::Forest)
    , __c(0)
    , __selected(-1)
    , selectingcrea(false)
    , selectingadv(true)
    , __count_before_flyaway(0)
    , can_escape(true)
    , life1(sf::Vector2f(LIFEBAR_WIDTH, LIFEBAR_HEIGHT))
    , life2(sf::Vector2f(LIFEBAR_WIDTH, LIFEBAR_HEIGHT))
    , ui_my_selected(0)
    , ui_enemy_selected(0)
    , attacking(false)
    , has_selected_an_atk(false)
    , atk_using_sort_of(-1)
    , attack_frames_count(0)
    , display_attack(false)
    , my_turn(true)
    , attacking_enemy(true)
{
}

bool FightView::load()
{
    sf::Texture bk1;
    if (!bk1.loadFromFile("assets/gui/fight/background1.png"))
        return false;
    this->textures.add("bk1", bk1);

    sf::Texture bk2;
    if (!bk2.loadFromFile("assets/gui/fight/background2.png"))
        return false;
    this->textures.add("bk2", bk2);

    sf::Texture bk3;
    if (!bk3.loadFromFile("assets/gui/fight/background3.png"))
        return false;
    this->textures.add("bk3", bk3);

    sf::Texture gr1;
    if (!gr1.loadFromFile("assets/gui/fight/ground1.png"))
        return false;
    this->textures.add("gr1", gr1);

    sf::Texture gr2;
    if (!gr2.loadFromFile("assets/gui/fight/ground2.png"))
        return false;
    this->textures.add("gr2", gr2);

    sf::Texture gr3;
    if (!gr3.loadFromFile("assets/gui/fight/ground3.png"))
        return false;
    this->textures.add("gr3", gr3);

    sf::Texture tool;
    if (!tool.loadFromFile("assets/gui/fight/bottom.png"))
        return false;
    this->textures.add("tool", tool);

    sf::Texture overlay;
    if (!overlay.loadFromFile("assets/gui/fight/overlay.png"))
        return false;
    this->textures.add("overlay", overlay);

    sf::Texture lifebar;
    if (!lifebar.loadFromFile("assets/gui/fight/lifebar.png"))
        return false;
    this->textures.add("lifebar", lifebar);

    sf::Texture bkg_select;
    if (!bkg_select.loadFromFile("assets/gui/fight/selectui.png"))
        return false;
    this->textures.add("select", bkg_select);

    this->sprites[this->BKG1] = sf::Sprite(this->textures.get("bk1"));
    this->sprites[this->BKG2] = sf::Sprite(this->textures.get("bk2"));
    this->sprites[this->BKG3] = sf::Sprite(this->textures.get("bk3"));
    this->sprites[this->GRD1] = sf::Sprite(this->textures.get("gr1"));
    this->sprites[this->GRD2] = sf::Sprite(this->textures.get("gr2"));
    this->sprites[this->GRD3] = sf::Sprite(this->textures.get("gr3"));
    this->sprites[this->TOOLS] = sf::Sprite(this->textures.get("tool"));
    this->sprites[this->OVERLAY] = sf::Sprite(this->textures.get("overlay"));
    this->sprites[this->LIFEBAR] = sf::Sprite(this->textures.get("lifebar"));
    this->sprites[this->LIFEBAR2] = sf::Sprite(this->textures.get("lifebar"));
    this->sprites[this->BKG_SELECT] = sf::Sprite(this->textures.get("select"));

    this->sprites[this->BKG1].setPosition(0.0f, 0.0f);
    this->sprites[this->BKG2].setPosition(0.0f, 0.0f);
    this->sprites[this->BKG3].setPosition(0.0f, 0.0f);
    this->sprites[this->OVERLAY].setPosition(0.0f, 0.0f);
    this->sprites[this->TOOLS].setPosition(0.0f, WIN_H - 150.0f);
    this->sprites[this->BKG_SELECT].setPosition(120.0f, 120.0f);
    this->sprites[this->LIFEBAR].setPosition(6.0f, 111.0f);
    this->sprites[this->LIFEBAR2].setPosition(507.0f, 348.0f);

    this->life1.setPosition(8.0f, 113.0f);
    this->life2.setPosition(509.0f, 350.0f);

    if (!this->font.loadFromFile(FONTPATH))
        return false;

    setupFont(this->action, this->font, sf::Color::Black, 24)
    this->action.setPosition(5.0f, WIN_H - 155.0f);
    this->action.setString("Ceci est un message d'action");

    setupFont(this->enemy, this->font, sf::Color::Black, 20)
    this->enemy.setPosition(6.0f, 86.0f);
    setupFont(this->me, this->font, sf::Color::Black, 20)
    this->me.setPosition(421.0f, 323.0f);

    setupFont(this->e_pv, this->font, sf::Color::Black, 20)
    this->e_pv.setPosition(188.0f, 110.0f);
    setupFont(this->m_pv, this->font, sf::Color::Black, 20)
    this->m_pv.setPosition(398.0f, 348.0f);

    setupFont(this->attack_name, this->font, sf::Color::Black, 20)

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
    window.draw(this->enemy);
    window.draw(this->me);

    // draw the creatures
    for (int i =0; i < this->adv.size(); ++i)
    {
        window.draw(this->sprites[this->__adv + to_string<int>(i)]);
    }
    for (int i=0; i < this->equip->getSize(); ++i)
    {
        window.draw(this->sprites[this->__me + to_string<int>(i)]);
    }

    // draw the life bars AND the "life" inside (life <-> color matching)
    // enemy
    float e_life = LIFEBAR_WIDTH * (float(this->adv[this->ui_enemy_selected]->getLife()) / float(this->adv[this->ui_enemy_selected]->getMaxLife()));
    this->life1.setSize(sf::Vector2f(e_life, LIFEBAR_HEIGHT));
    if (e_life / LIFEBAR_WIDTH <= 0.33f)
        this->life1.setFillColor(sf::Color(220, 40, 20));
    else if (e_life / LIFEBAR_WIDTH <= 0.66f)
        this->life1.setFillColor(sf::Color(255, 130, 50));
    else
        this->life1.setFillColor(sf::Color(40, 220, 20));
    window.draw(this->life1);
    window.draw(this->sprites[this->LIFEBAR]);
    window.draw(this->e_pv);
    // me
    float m_life = LIFEBAR_WIDTH * (float(this->equip->getCrea(this->ui_my_selected)->getLife()) / float(this->equip->getCrea(this->ui_my_selected)->getMaxLife()));
    this->life2.setSize(sf::Vector2f(m_life, LIFEBAR_HEIGHT));
    if (m_life / LIFEBAR_WIDTH <= 0.33f)
        this->life2.setFillColor(sf::Color(220, 40, 20));
    else if (m_life / LIFEBAR_WIDTH <= 0.66f)
        this->life2.setFillColor(sf::Color(255, 130, 50));
    else
        this->life2.setFillColor(sf::Color(40, 220, 20));
    window.draw(this->life2);
    window.draw(this->sprites[this->LIFEBAR2]);
    window.draw(this->m_pv);

    // draw interface to select an attack
    if (this->attacking)
    {
        window.draw(this->sprites[this->BKG_SELECT]);
        this->attack_name.setString("Sortilèges disponnibles");
        this->attack_name.setPosition(120.0f + float(int((400.0f - this->attack_name.getGlobalBounds().width) / 2.0f)), 130.0f);
        window.draw(this->attack_name);
        for (int i=0; i < this->equip->getSize(); ++i)
        {
            /// CHANGE THIS LINE IF WE ADD COMPETENCES' TREE, TO SET THE NAME OF THE ATTACK REGARDING
            /// TO THE TREE OF THE `SORT`
            /// afficher le type de sort
            this->attack_name.setString(std::string("Sort de : ") + this->equip->getCrea(i)->getName());
            if (this->attacks_used[i])
                this->attack_name.setColor(sf::Color::Green);
            else
                this->attack_name.setColor(sf::Color::Black);
            this->attack_name.setPosition(140.0f, 140.0f + SPACING_ATK_LISTING_Y * i);
        }
    }

    // draw interface to select a crea
    if (this->selectingcrea)
    {
        window.draw(this->sprites[this->BKG_SELECT]);

        if (this->selectingadv)
        {
            for (int i=0; i < this->adv.size(); i++)
            {
                if (i != this->__selected)
                    this->texts.get(this->__adv + to_string<int>(i)).setFillColor(sf::Color::Black);
                else
                    this->texts.get(this->__adv + to_string<int>(i)).setFillColor(sf::Color::Green);

                window.draw(this->texts.get(this->__adv + to_string<int>(i)));
            }
        }
        else
        {
            for (int i=0; i < this->equip->getSize(); ++i)
            {
                if (i != this->__selected)
                    this->texts.get(this->__me + to_string<int>(i)).setFillColor(sf::Color::Black);
                else
                    this->texts.get(this->__me + to_string<int>(i)).setFillColor(sf::Color::Green);

                window.draw(this->texts.get(this->__me + to_string<int>(i)));
            }
        }
    }

    // displaying particules for the attack
    if (this->display_attack)
    {
        //
    }
}

int FightView::process_event(sf::Event& event, sf::Time elapsed)
{
    int new_view = -1, m = 0, pos_atk_sel = 0;

    if (this->__count_before_flyaway == 0)  // disable controls when escaping
    {
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
                if (!this->selectingcrea && !this->attacking && this->my_turn)
                {
                    // click on the buttons sections
                    if (m__Y >= 490 + 75 && m__Y <= 490 + 125)
                    {
                        if (m__X >= 5 && m__X <= 157)
                        {
                            // attack button
                            this->attacking = true;
                            for (int i=0; i < this->equip->getSize(); ++i)
                            {
                                this->attacks_used[i] = false;
                            }
                        }
                        else if (m__X >= 164 && m__X <= 316)
                        {
                            // equip button
                            new_view = MYCREATURES_VIEW_ID;
                            OMessenger::setlock(MYCREATURES_VIEW_ID);
                        }
                        else if (m__X >= 323 && m__X <= 475)
                        {
                            // bag button
                            new_view = INVENTORY_VIEW_ID;
                            OMessenger::setlock(INVENTORY_VIEW_ID);
                        }
                        else if (m__X >= 480 && m__X <= 632)
                        {
                            // fly away button
                            this->__count_before_flyaway = 220;
                            if (this->can_escape)
                                this->action.setString("Vous vous échapez prestement ...");
                            else
                                this->action.setString("Vous ne pouvez pas vous échaper !");
                        }
                    }
                    // click on one of my creatures
                    else if (458 - CREATURE_HEIGHT <= m__Y && 458 <= m__Y && 47 <= m__X )
                    {
                        // this->sprites[this->__me + to_string<int>(i)].setPosition(47.0f + i * SPACEMENT_X, 458.0f - CREATURE_HEIGHT);
                        this->ui_my_selected = int((m__X - 47) / SPACEMENT_X) % this->equip->getSize();
                    }
                    // click on a creature which belongs to the enemy
                    else if (206 - CREATURE_HEIGHT <= m__Y && m__Y <= 206 && START_X <= m__X)
                    {
                        // this->sprites[this->__adv + to_string<int>(i)].setPosition(START_X + i * SPACEMENT_X, 206.0f - CREATURE_HEIGHT);
                        this->ui_enemy_selected = int((m__X - START_X) / SPACEMENT_X) % this->adv.size();
                    }
                }
                else if (this->selectingcrea)
                {
                    // handle click in
                    if (m__X >= X_TEXT_SELCREA_UI && m__X <= MX_TEXT_SELCREA_UI)
                    {
                        if (m__Y >= Y_TEXT_SELCREA_UI && m__Y <= MY_TEXT_SELCREA_UI)
                        {
                            this->__selected = int((m__Y - Y_TEXT_SELCREA_UI) / YS_TEXT_SELCREA_UI);
                            m = (this->selectingadv) ? this->adv.size() : this->equip->getSize();
                            if (this->__selected >= 0 && this->__selected < m)
                                this->selectingcrea = false;
                            else
                                this->__selected = -1;
                        }
                    }
                }
                else if (this->attacking)
                {
                    // handle click in attack selection ui
                    pos_atk_sel = int((m__Y - 140) / SPACING_ATK_LISTING_Y);
                    if (0 <= pos_atk_sel && pos_atk_sel < this->equip->getSize())
                    {
                        this->atk_using_sort_of = pos_atk_sel;
                        this->attacks_used[pos_atk_sel] = true;
                        SortilegeType s = this->equip->getCrea(this->atk_using_sort_of)->getSort()->getType();
                        this->attacking_enemy = !(s == UniqueTargetUsHeal || s == MultipleUsHeal || s == MultipleUsHealStatus);
                        this->selectingcrea = true;
                        this->selectingadv = this->attacking_enemy;
                        this->action.setString("Sur quelle créature souhaitez-vous utiliser le sort ?");
                        this->has_selected_an_atk = true;
                        this->display_attack = true;
                        this->attack_frames_count = 120;
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
    }

    if (this->__count_before_flyaway == 1)
    {
        this->__count_before_flyaway = 0;
        if (this->can_escape)
            return DEFAULT_VIEW_ID;
        else
            return -1;
    }
    return new_view;
}

void FightView::update(sf::RenderWindow& window, sf::Time elapsed)
{
    // updating the number of frames in which we "display" the attack
    if (this->attack_frames_count > 0)
        this->attack_frames_count -= 1;
    if (this->attack_frames_count == 1)
    {
        this->attack_frames_count = 0;
        this->display_attack = false;
    }

    // updating countdown before quitting duel
    if (this->__count_before_flyaway > 1)
        this->__count_before_flyaway -= 1;

    if (OMessenger::get().target_view == this->getId())
    {
        switch (OMessenger::get().type)
        {
        case ObjType::capture:
            this->__c = OMessenger::get().value;
            break;

        default:
            // should never land here
            DebugLog(SH_WARN, "We land here but we should'nt. What type of object is it ? => " << OMessenger::get().type);
            break;
        }

        this->selectingcrea = true;
        this->selectingadv = true;

        // capturing
        if (this->__c && this->__selected != -1)
        {
            OMessenger::empty();
            if ((rand() % 101) >= this->__c)
            {
                // we got the creature let's register it
                this->dex->register_capture(this->adv[this->__selected]->getId());
                if (!this->equip->add_creature(this->adv[this->__selected]))
                {
                    this->pc->add_creature(this->adv[this->__selected]);
                    this->action.setString("La créature a été envoyée au PC");
                }
                else
                    this->action.setString("La créature a été ajoutée à l'équipe !");
            }
            else
                this->action.setString("La créature n'a pas pu être attrapée ...");
            this->__c = 0;
            this->__selected = -1;
        }
    }

    // updating stats (name, PV)
    this->enemy.setString(this->adv[this->ui_enemy_selected]->getName());
    this->me.setString(this->equip->getCrea(this->ui_my_selected)->getName());
    this->e_pv.setString(to_string<int>(this->adv[this->ui_enemy_selected]->getLife()) + "/" + to_string<int>(this->adv[this->ui_enemy_selected]->getMaxLife()));
    this->m_pv.setString(to_string<int>(this->equip->getCrea(this->ui_my_selected)->getLife()) + "/" + to_string<int>(this->equip->getCrea(this->ui_my_selected)->getMaxLife()));

    // check if it is still our turn to play
    if (this->my_turn)
    {
        int c = 0;
        for (int i=0; i < this->attacks_used.size(); ++i)
        {
            if (this->attacks_used[i])
                ++c;
        }
        if (c == this->attacks_used.size())
        {
            // it is not our turn anymore, let the AI play !
            this->my_turn = false;
        }
    }

    // attack !
    if (this->has_selected_an_atk && this->__selected != -1 && !this->selectingcrea && this->my_turn)
    {
        this->has_selected_an_atk = false;
        this->attack(this->__selected, this->atk_using_sort_of);
    }
}

void FightView::attack(int selected, int index_my_creatures)
{
    /// our creature attacking
    this->equip->getCrea(index_my_creatures);
    /// the enemy
    if (this->attacking_enemy)
        this->adv[selected];
    else
        this->equip->getCrea(selected);
    /// apply sort + type's damages + def ... etc

    /// WHEN DUEL IS FINISHED, GIVE EXP OR FLY AWAY TO HEAL THE CREATURES
}

void FightView::encounter()
{
    if (this->dex)
    {
        for (int i=0; i < this->adv.size(); i++)
            this->dex->register_viewed(this->adv[i]->getId());
    }
}

void FightView::set_env(FightEnv new_env)
{
    this->env = new_env;
}

void FightView::set_dex(Dex* _dex)
{
    this->dex = _dex;
}

void FightView::set_equip(Equip* eq)
{
    this->equip = eq;
    this->attacks_used.reserve(this->equip->getSize());
}

void FightView::set_pc(Equip* pc)
{
    this->pc = pc;
}

void FightView::set_crealoader(CreaturesLoader* creal)
{
    this->crealoader = creal;
}

void FightView::start()
{
    CLEAR_PTR_VECT(this->adv)
    this->adv.clear();

    // generate adv
    float moy_equip = 0.0f;
    for (int i=0; i < this->equip->getSize(); ++i)
    {
        sf::Text _t1;
        setupFont(_t1, this->font, sf::Color::Black, 24)
        _t1.setString(this->equip->getCrea(i)->getName());
        _t1.setPosition(X_TEXT_SELCREA_UI, Y_TEXT_SELCREA_UI + i * YS_TEXT_SELCREA_UI);
        this->texts.add(this->__me + to_string<int>(i), _t1);

        moy_equip += this->equip->getCrea(i)->getLevel();

        this->sprites[this->__me + to_string<int>(i)] = sf::Sprite(this->crealoader->get(this->dex->getInfo(this->equip->getCrea(i)->getId()).file));
        float factor = CREATURE_HEIGHT / this->crealoader->get(this->dex->getInfo(this->equip->getCrea(i)->getId()).file).getSize().y;
        this->sprites[this->__me + to_string<int>(i)].setScale(factor, factor);
        // reversing our creatures to make them look to the right (to the others creatures)
        int width = this->sprites[this->__me + to_string<int>(i)].getGlobalBounds().width,
             height = this->sprites[this->__me + to_string<int>(i)].getGlobalBounds().height;
        this->sprites[this->__me + to_string<int>(i)].setTextureRect(sf::IntRect(width, 0, -width, height));
        // calculate position of the sprite regarding to the index
        this->sprites[this->__me + to_string<int>(i)].setPosition(47.0f + i * SPACEMENT_X, 458.0f - CREATURE_HEIGHT);
    }
    moy_equip /= float(this->equip->getSize());

    int _x_ = rand() % 10 + ((moy_equip - 4 > 0) ? int(moy_equip) - 4 : int(moy_equip), moy_equip + 6);
    _x_ = (_x_ - 2 > 0) ? _x_ - 2 : _x_;

    for (int i=0; i < this->equip->getMaxSize(); i++)
    {
        Creature* crea = new Creature();
        int  id = rand() % (this->dex->getMaxId() + 1)
              , _t = this->dex->getInfo(id).type
              ,  _s = this->dex->getInfo(id).stade
              , _st = rand() % SortilegeType::Count  // SortilegeType::UniqueTargetAdvDamage
              , level = (rand() % 4) + _x_
              , life = 2 * level + (rand() % 4) + 3  // mlife = life
              , pp = Creature::calculatePPFromLevel(level) // mpp = pp
              , sdmg = ceil(((rand() % 4) + 3) * 0.125 * level + 1)  // damages for the sortilege
              , stargets = rand() % (this->equip->getSize()+ 1)  // targets for the sortilege
              , atk = Creature::calculateStatFromLevel(level)  // attack of the creature
              , def = Creature::calculateStatFromLevel(level);  // defense
        long int exp = Creature::calculateExpFromLevel(level);

        Type t = static_cast<Type>(_t % 8);
        State s = static_cast<State>(_s % 4);
        SortilegeType st = static_cast<SortilegeType>(_st % 14);

        crea->load(id, t, atk, def, life, life, pp, pp, this->dex->getName(id), s, level, exp, st, sdmg, stargets);
        this->adv.push_back(crea);

        sf::Text _t2;
        setupFont(_t2, this->font, sf::Color::Black, 24)
        _t2.setString(this->dex->getName(id));
        _t2.setPosition(X_TEXT_SELCREA_UI, Y_TEXT_SELCREA_UI + i * YS_TEXT_SELCREA_UI);
        this->texts.add(this->__adv + to_string<int>(i), _t2);

        this->sprites[this->__adv + to_string<int>(i)] = sf::Sprite(this->crealoader->get(this->dex->getInfo(crea->getId()).file));
        float factor = CREATURE_HEIGHT / this->crealoader->get(this->dex->getInfo(crea->getId()).file).getSize().y;
        this->sprites[this->__adv + to_string<int>(i)].setScale(factor, factor);
        // calculate position of the sprite regarding to the index
        this->sprites[this->__adv + to_string<int>(i)].setPosition(START_X + i * SPACEMENT_X, 206.0f - CREATURE_HEIGHT);
    }

    this->encounter();
}

void FightView::set_escape(bool v)
{
    this->can_escape = v;
}

FightView::~FightView()
{
    CLEAR_PTR_VECT(this->adv)
    this->dex = NULL;
    this->equip = NULL;
    this->pc = NULL;
    this->crealoader = NULL;
}
