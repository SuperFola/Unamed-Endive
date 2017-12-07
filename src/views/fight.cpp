#include <iostream>
#include <random>
#include <ctime>
#include <cstdlib>
#include "../../debug.hpp"

#include "fight.hpp"
#include "../constants.hpp"

std::string convert_sort(SortilegeType s)
{
    std::string o = "null";
    switch (s)
    {
    // unique adv
    case SortilegeType::UniqueTargetAdvDamage:
        o = "attaque";
        break;

    case SortilegeType::UniqueTargetAdvPoison:
        o = "empoisonne";
        break;

    case SortilegeType::UniqueTargetAdvBurn:
        o = "brûle";
        break;

    case SortilegeType::UniqueTargetAdvParalize:
        o = "paralyse";
        break;

    // unique us
    case SortilegeType::UniqueTargetUsHeal:
        o = "soigne";
        break;

    // multiple adv
    case SortilegeType::MultipleAdvDamage:
        o = "[multi] attaque";
        break;

    case SortilegeType::MultipleAdvPoison:
        o = "[multi] empoisonne";
        break;

    case SortilegeType::MultipleAdvBurn:
        o = "[multi] brûle";
        break;

    case SortilegeType::MultipleAdvParalize:
        o = "[multi] paralyse";
        break;

    // multiple us
    case SortilegeType::MultipleUsHeal:
        o = "[multi] soigne";
        break;

    case SortilegeType::MultipleUsHealStatus:
        o = "[multi] soigne statuts";
        break;
    }

    return o;
}

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
    , particles(2048)
    , eq_x(0.0f)
    , eq_y(0.0f)
    , ending(0)
    , enemy_is_attacking(false)
    , enemy_wait_until_next(0)
    , lock(false)
    , wait_give_xp(0)
    , whoisdead(NODEAD)
    , iamattacking(false)
    , wait(0)
    , giving_xp_to(-1)
    , random_encounter(true)
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
    this->action.setString("C'est à votre tour d'attaquer !");

    setupFont(this->enemy, this->font, sf::Color::Black, 20)
    this->enemy.setPosition(6.0f, 86.0f);
    setupFont(this->me, this->font, sf::Color::Black, 20)
    this->me.setPosition(421.0f, 323.0f);

    setupFont(this->e_pv, this->font, sf::Color::Black, 20)
    this->e_pv.setPosition(188.0f, 112.0f);
    setupFont(this->m_pv, this->font, sf::Color::Black, 20)
    this->m_pv.setPosition(398.0f, 350.0f);

    setupFont(this->attack_name, this->font, sf::Color::Black, 20)

    if (!this->black_fade.create(WIN_W, WIN_W))
        return false;
    sf::Uint8* pixels = new sf::Uint8[WIN_W * WIN_W * 4];
    for (unsigned int i=0; i < WIN_W * WIN_H * 4; ++i) { pixels[i] = 0; }
    this->black_fade.update(pixels);
    this->black_fade_sprite.setTexture(this->black_fade);
    this->black_fade_sprite.setColor(sf::Color(0, 0, 0, 255));
    this->black_fade_sprite.setPosition(0.0f, 0.0f);

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

    // draw the creatures
    for (int i=0; i < this->adv.size(); ++i)
    {
        if (this->adv[i]->getLife() > 0 || (this->display_attack && this->adv[i]->getLife() <= 0
                                            && std::find(this->cibles.begin(), this->cibles.end(), i) != this->cibles.end()
                                            && this->ending == 0))
            window.draw(this->sprites[this->__adv + to_string<int>(i)]);
    }
    for (int i=0; i < this->equip->getSize(); ++i)
    {
        if (this->equip->getCrea(i)->getLife() > 0 || (this->display_attack && this->equip->getCrea(i)->getLife() <= 0
                                                       && std::find(this->cibles.begin(), this->cibles.end(), -i - 1) != this->cibles.end()
                                                       && this->ending == 0))
            window.draw(this->sprites[this->__me + to_string<int>(i)]);
    }

    // draw the life bars AND the "life" inside (life <-> color matching) AND state (burned ...)
    // enemy
    {
        window.draw(this->enemy);
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
        window.draw(this->me);
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
    }

    // draw interface to select an attack
    if (this->attacking)
    {
        window.draw(this->sprites[this->BKG_SELECT]);
        this->attack_name.setString("Sortilèges disponibles");
        this->attack_name.setPosition(120.0f + float(int((400.0f - this->attack_name.getGlobalBounds().width) / 2.0f)), 130.0f);
        window.draw(this->attack_name);
        for (int i=0; i < this->equip->getSize(); ++i)
        {
            /// CHANGE THIS LINE IF WE ADD COMPETENCES' TREE, TO SET THE NAME OF THE ATTACK REGARDING
            /// TO THE TREE OF THE `SORT`
            this->attack_name.setString(std::string("(") + convert_sort(this->equip->getCrea(i)->getSort()->getType()) + ") " + this->equip->getCrea(i)->getName());

            if (this->equip->getCrea(i)->getLife() > 0)
            {
                if (this->attacks_used[i])
                    this->attack_name.setFillColor(sf::Color::Red);
                else
                    this->attack_name.setFillColor(sf::Color::Black);
                this->attack_name.setPosition(X_TEXT_SELCREA_UI, Y_TEXT_SELCREA_UI + i * SPACING_ATK_LISTING_Y);
                window.draw(this->attack_name);
            }
        }
    }

    // draw interface to select a crea
    if (this->selectingcrea)
    {
        window.draw(this->sprites[this->BKG_SELECT]);
        this->attack_name.setString("Cible");
        this->attack_name.setPosition(120.0f + float(int((400.0f - this->attack_name.getGlobalBounds().width) / 2.0f)), 130.0f);
        this->attack_name.setFillColor(sf::Color::Black);
        window.draw(this->attack_name);
        if (this->selectingadv)
        {
            for (int i=0; i < this->adv.size(); ++i)
            {
                if (this->adv[i]->getLife() > 0)
                {
                    if (i != this->__selected)
                        this->texts.get(this->__adv + to_string<int>(i)).setFillColor(sf::Color::Black);
                    else
                        this->texts.get(this->__adv + to_string<int>(i)).setFillColor(sf::Color::Green);

                    window.draw(this->texts.get(this->__adv + to_string<int>(i)));
                }
            }
        }
        else
        {
            for (int i=0; i < this->equip->getSize(); ++i)
            {
                if (this->equip->getCrea(i)->getLife() > 0)
                {
                    if (i != this->__selected)
                        this->texts.get(this->__me + to_string<int>(i)).setFillColor(sf::Color::Black);
                    else
                        this->texts.get(this->__me + to_string<int>(i)).setFillColor(sf::Color::Green);

                    window.draw(this->texts.get(this->__me + to_string<int>(i)));
                }
            }
        }
    }

    // animation
    if (this->ending)
    {
        window.draw(this->black_fade_sprite);
    }

    // displaying particules for the attack
    if (this->display_attack)
    {
        window.draw(this->particles);
    }
}

int FightView::process_event(sf::Event& event, sf::Time elapsed)
{
    int new_view = -1, m = 0, pos_atk_sel = 0, t = 0;

    if (this->__count_before_flyaway == 0 && this->ending == 0)  // disable controls when escaping or quitting
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
                // to control the UI, it must be our turn, AND not attacking
                if (!this->selectingcrea && !this->attacking && this->my_turn)
                {
                    // click on the buttons sections
                    if (m__Y >= 490 + 75 && m__Y <= 490 + 125)
                    {
                        if (m__X >= 5 && m__X <= 157)
                        {
                            // attack button
                            this->attacking = true;
                            this->attacks_used.clear();
                            for (int i=0; i < this->equip->getSize(); i++)
                            {
                                this->attacks_used.push_back(false);
                            }
                            DebugLog(SH_INFO, "clic on attack");
                        }
                        else if (m__X >= 164 && m__X <= 316)
                        {
                            // equip button
                            new_view = MYCREATURES_VIEW_ID;
                            OMessenger::setlock(MYCREATURES_VIEW_ID);
                            DebugLog(SH_INFO, "clic on equip");
                        }
                        else if (m__X >= 323 && m__X <= 475)
                        {
                            // bag button
                            new_view = INVENTORY_VIEW_ID;
                            OMessenger::setlock(INVENTORY_VIEW_ID);
                            DebugLog(SH_INFO, "clic on bag");
                        }
                        else if (m__X >= 480 && m__X <= 632)
                        {
                            // fly away button
                            this->__count_before_flyaway = 220;
                            if (this->can_escape)
                                this->action.setString("Vous vous échappez prestement ...");
                            else
                                this->action.setString("Vous ne pouvez pas vous échapper !");
                            DebugLog(SH_INFO, "clic on escape");
                        }
                    }
                    // click on one of my creatures
                    else if (458 - CREATURE_HEIGHT <= m__Y && 458 <= m__Y && 47 <= m__X )
                    {
                        // this->sprites[this->__me + to_string<int>(i)].setPosition(47.0f + i * SPACEMENT_X, 458.0f - CREATURE_HEIGHT);
                        t = int((m__X - 47) / SPACEMENT_X) % this->equip->getSize();
                        if (this->equip->getCrea(t)->getLife() > 0)
                        {
                            this->ui_my_selected = t;
                            DebugLog(SH_INFO, "selected my creature : " << this->equip->getCrea(t)->getName());
                        }
                    }
                    // click on a creature which belongs to the enemy
                    else if (206 - CREATURE_HEIGHT <= m__Y && m__Y <= 206 && START_X <= m__X)
                    {
                        // this->sprites[this->__adv + to_string<int>(i)].setPosition(START_X + i * SPACEMENT_X, 206.0f - CREATURE_HEIGHT);
                        t = int((m__X - START_X) / SPACEMENT_X) % this->adv.size();
                        if (this->adv[t]->getLife() > 0)
                        {
                            this->ui_enemy_selected = t;
                            DebugLog(SH_INFO, "selected an enemy : " << this->adv[t]->getName());
                        }
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

                            if (this->selectingadv && this->adv[this->__selected]->getLife() == 0)
                                this->__selected = -1;
                            if (!this->selectingadv && this->equip->getCrea(this->__selected)->getLife() == 0)
                                this->__selected = -1;

                            DebugLog(SH_INFO, "selectingcrea=true ; __selected=" << this->__selected);
                        }
                    }
                }
                else if (this->attacking)
                {
                    if ((0 <= m__X && m__X <= 120) || (WIN_W - 120 <= m__X && m__X <= WIN_W) ||
                         (0 <= m__Y && m__Y <= 120) || (WIN_H - 120 <= m__Y && m__Y <= WIN_H))
                    {
                            this->attacking = false;
                            DebugLog(SH_INFO, "attacking=false");
                    }
                    else
                    {
                        // handle click in attack selection ui
                        pos_atk_sel = int((m__Y - Y_TEXT_SELCREA_UI) / SPACING_ATK_LISTING_Y);
                        if (0 <= pos_atk_sel && pos_atk_sel < this->equip->getSize() && this->equip->getCrea(pos_atk_sel)->getLife() > 0)
                        {
                            this->atk_using_sort_of = pos_atk_sel;
                            this->attacks_used[pos_atk_sel] = true;

                            SortilegeType s = this->equip->getCrea(this->atk_using_sort_of)->getSort()->getType();
                            this->attacking_enemy = !(s == UniqueTargetUsHeal || s == MultipleUsHeal || s == MultipleUsHealStatus);
                            if (s == UniqueTargetAdvDamage || s == UniqueTargetAdvPoison || s == UniqueTargetAdvBurn ||
                                 s == UniqueTargetAdvParalize || s == UniqueTargetUsHeal)
                            {
                                this->selectingcrea = true;
                                this->selectingadv = this->attacking_enemy;
                                this->action.setString("Sur quelle créature souhaitez-vous utiliser le sort ?");
                            }
                            else
                            {
                                this->__selected = 42;  // special code to tell the engine to chose random creatures
                                this->action.setString("Attaque multiple déclenchée");
                                this->display_attack = true;
                                DebugLog(SH_INFO, "display_attack=true");
                                this->attack_frames_count = FightView::ATK_FR_CNT();
                            }

                            DebugLog(SH_INFO, "clic in attack selection ui ; " << pos_atk_sel);

                            this->has_selected_an_atk = true;
                            this->lock = true;
                            this->attacking = false;
                        }
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
        DebugLog(SH_INFO, "flying away");
        if (this->can_escape)
            return DEFAULT_VIEW_ID;
        else
            return -1;
    }
    if (this->ending == 1)
    {
        DebugLog(SH_INFO, "ending");
        this->ending = 0;
        this->on_end();
        return DEFAULT_VIEW_ID;
    }
    return new_view;
}

void FightView::update(sf::RenderWindow& window, sf::Time elapsed)
{
    // updating the number of frames in which we "display" the attack
    if (this->attack_frames_count > 1 && !this->lock)
        --this->attack_frames_count;
    if (this->attack_frames_count == 1)
    {
        this->attack_frames_count = 0;
        this->display_attack = false;
        DebugLog(SH_WARN, "display_attack=false");
        this->iamattacking = false;
    }

    if (this->wait_give_xp > 1)
        --this->wait_give_xp;
    if (this->wait_give_xp == 1)
    {
        this->wait_give_xp = 0;
        /// just to be sure
        this->display_attack = false;
        DebugLog(SH_WARN, "display_attack=false");
        this->cibles.clear();
        this->ending = ENDING_CNT;
        DebugLog(SH_SPE, "wait_give_xp=0, starting to decrement ending");
        if (this->whoisdead == DEADOTH)
            this->action.setString("Les créatures ennemies ont perdues !");
        else if (this->whoisdead == DEADME)
            this->action.setString("Vous avez perdu ...");
    }

    // updating the number of frames during which the AI attacks
    if (this->enemy_wait_until_next > 1 && !this->lock)
        --this->enemy_wait_until_next;
    if (this->enemy_wait_until_next == 1)
    {
        this->enemy_wait_until_next = 0;
    }

    if (this->wait > 1)
        --this->wait;
    if (this->wait == 1)
        this->wait = 0;

    // updating countdown before quitting duel
    if (this->__count_before_flyaway > 1)
        --this->__count_before_flyaway;

    if (this->ending > 1 && this->wait_give_xp == 0 && this->wait == 0)
        --this->ending;

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
                    pop(&this->adv, this->__selected);
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
    {
        std::string s;

        s = this->adv[this->ui_enemy_selected]->getStringState();
        if (s != "none")
            this->enemy.setString(this->adv[this->ui_enemy_selected]->getName() + " (" + s + ") niv." + to_string<int>(this->adv[this->ui_enemy_selected]->getLevel()));
        else
            this->enemy.setString(this->adv[this->ui_enemy_selected]->getName() + " niv." + to_string<int>(this->adv[this->ui_enemy_selected]->getLevel()));

        s = this->equip->getCrea(this->ui_my_selected)->getStringState();
        if (s != "none")
            this->me.setString(this->equip->getCrea(this->ui_my_selected)->getName() + " (" + s + ") niv." + to_string<int>(this->equip->getCrea(this->ui_my_selected)->getLevel()));
        else
            this->me.setString(this->equip->getCrea(this->ui_my_selected)->getName() + " niv." + to_string<int>(this->equip->getCrea(this->ui_my_selected)->getLevel()));
        this->e_pv.setString(to_string<int>(this->adv[this->ui_enemy_selected]->getLife()) + "/" + to_string<int>(this->adv[this->ui_enemy_selected]->getMaxLife()));
        this->m_pv.setString(to_string<int>(this->equip->getCrea(this->ui_my_selected)->getLife()) + "/" + to_string<int>(this->equip->getCrea(this->ui_my_selected)->getMaxLife()));
    }

    // changing the current selected creature to display its stats if it is dead, to a living one
    {
        int c = 0;
        while (this->adv[this->ui_enemy_selected]->getLife() == 0)
        {
            this->ui_enemy_selected = (this->ui_enemy_selected + 1) % this->adv.size();

            if (++c == this->adv.size())
                break;
        }
        c = 0;
        while (this->equip->getCrea(this->ui_my_selected)->getLife() == 0)
        {
            this->ui_my_selected = (this->ui_my_selected + 1) % this->equip->getSize();

            if (++c == this->adv.size())
                break;
        }
    }

    // check if it is still our turn to play
    if (this->my_turn)
    {
        int c = 0;
        for (int i=0; i < this->attacks_used.size(); i++)
        {
            if (this->attacks_used[i])
                ++c;
        }
        if (c == this->attacks_used.size() && !this->lock && !this->display_attack && this->attack_frames_count == 0)
        {
            // it is not our turn anymore, let the AI play !
            this->my_turn = false;
            this->selectingcrea = false;
            this->attacking = false;
            this->__selected = -1;
            this->action.setString("Votre adversaire attaque !");
        }
    }

    // attack !
    if (this->has_selected_an_atk && this->__selected != -1 && this->my_turn)
    {
        DebugLog(SH_INFO, "You are attacking");
        this->cibles.clear();
        DebugLog(SH_SPE, "----------------------------");
        this->attack(this->__selected, this->atk_using_sort_of);
        for (int i=0; i < this->cibles.size(); ++i) {DebugLog(SH_SPE, "cibles[" << i << "]=" << this->cibles[i]);}
        DebugLog(SH_SPE, "----------------------------");
        this->__selected = -1;
        this->has_selected_an_atk = false;
        this->selectingcrea = false;
        this->lock = false;
        this->iamattacking = true;
    }
    else if (!this->my_turn && !this->enemy_is_attacking && !this->lock && !this->iamattacking)
    {
        // the AI must attack
        DebugLog(SH_INFO, "Turning on the AI");
        this->enemy_is_attacking = true;
        int c = 0;
        for (int i=0; i < this->adv.size(); ++i)
        {
            if (this->adv[i]->getLife() > 0)
                ++c;
        }
        this->enemy_wait_until_next = c * FightView::ATK_FR_CNT();
        this->check_statuses();
    }

    if (this->enemy_is_attacking)
    {
        if ((this->enemy_wait_until_next % FightView::ATK_FR_CNT()) == 0 && this->enemy_wait_until_next != 0 && !this->lock)
        {
            DebugLog(SH_WARN, "enemy attacking");
            // counting dead ones
            int c = 0;
            for (int i=0; i < this->adv.size(); ++i)
            {
                if (this->adv[i]->getLife() > 0)
                    ++c;
            }
            int choosen = c - int(this->enemy_wait_until_next / FightView::ATK_FR_CNT()) + 1;
            int c_alive = 0;
            for (int i=0; i < this->adv.size(); ++i)
            {
                if (this->adv[i]->getLife() > 0)
                    ++c_alive;

                if (c_alive == choosen)
                {
                    choosen = i;
                    break;
                }
            }
            this->cibles.clear();
            DebugLog(SH_SPE, "----------------------------");
            if (c_alive == 0)
                DebugLog(SH_WARN, "Found no creatures alive to attack, that's not normal !");
            else
                this->e_attack(choosen);
            for (int i=0; i < this->cibles.size(); ++i) {DebugLog(SH_SPE, "cibles[" << i << "]=" << this->cibles[i]);}
            DebugLog(SH_SPE, "----------------------------");
        }

        if (this->enemy_wait_until_next == 0)
        {
            DebugLog(SH_INFO, "Giving back the turn to the player");
            this->enemy_is_attacking = false;
            this->my_turn = true;
            for (int i=0; i < this->attacks_used.size(); ++i)
            {
                this->attacks_used[i] = false;
            }
            this->lock = true;
            this->action.setString("C'est à votre tour d'attaquer !");
            this->check_statuses();
        }
    }

    // check if we are dead or if it is the enemy who's dead
    {
        int d = 0;
        for (int i=0; i < this->adv.size(); ++i)
        {
            if (this->adv[i]->getLife() == 0)
                ++d;
        }
        if (d == this->adv.size() && this->adv.size() != 0 && this->ending == 0)
        {
            this->ending = ENDING_CNT;
            this->lock = true;
            this->give_xp(true);
            this->whoisdead = DEADOTH;
        }
        d = 0;
        for (int i=0; i < this->equip->getSize(); ++i)
        {
            if (this->equip->getCrea(i)->getLife() == 0)
                ++d;
        }
        if (d == this->equip->getSize() && this->equip->getSize() != 0 && this->ending == 0)
        {
            DebugLog(SH_WARN, "ok");
            this->ending = ENDING_CNT;
            this->my_turn = false;
            this->enemy_is_attacking = false;
            this->enemy_wait_until_next = 0;
            this->attacking = false;
            this->selectingcrea = false;
            this->__selected = -1;
            this->lock = true;
            this->give_xp(false);
            this->whoisdead = DEADME;
        }
    }

    // updating particle emitter for the attack
    if (this->display_attack)
    {
        this->particles.setEmitter(sf::Vector2f(this->eq_x, this->eq_y));
        this->particles.update(elapsed);
    }

    if (this->ending)
    {
        this->black_fade_sprite.setColor(sf::Color(0, 0, 0, int(this->ending / ENDING_CNT)));
    }

    if (this->wait_give_xp % 360 == 0 && this->wait_give_xp != 0 && this->giving_xp_to != -1)
    {
        int i = this->equip->getSize() - (this->wait_give_xp / 360);
        int m = (this->giving_xp_to == 1) ? this->equip->getSize() : this->adv.size();

        if (i < m)
        {
            DebugLog(SH_INFO, "giving xp to no " << i << ", giving_xp_to=" << this->giving_xp_to);
            if (this->giving_xp_to == 1)
            {
                int t = this->equip->getCrea(i % this->equip->getSize())->gainExp(this->adv[i % this->adv.size()]);
                this->action.setString(this->equip->getCrea(i)->getName() + " a gagné " + to_string<int>(t) + " niveau" + ((t == 1) ? std::string("") : std::string("x")));
            }
            else
            {
                int t = this->adv[i % this->adv.size()]->gainExp(this->equip->getCrea(i % this->equip->getSize()));
                this->action.setString(this->equip->getCrea(i)->getName() + " a perdu " + to_string<int>(t) + " niveau" + ((t == 1) ? std::string("") : std::string("x")));
            }
        }
    }
}

void FightView::give_xp(bool me)
{
    this->giving_xp_to = me ? 1 : 0;
    this->wait_give_xp = this->equip->getSize() * 360;
    DebugLog(SH_SPE, "wait_give_xp=" << this->wait_give_xp);
}

void FightView::check_statuses()
{
    // updating statuses
    std::string s = "";
    for (int i=0; i < this->adv.size(); ++i)
    {
        int r = this->adv[i]->updateState();
        switch(r)
        {
        case DEAD_BUR:
            s += std::string("L'ennemi ") + this->adv[i]->getName() + " est mort de sa brulure\n";
            break;

        case DEAD_PSN:
            s += std::string("L'ennemi ") + this->adv[i]->getName() + " est mort empoisonné\n";
            break;

        case LOST_BUR:
            s += std::string("L'ennemi ") + this->adv[i]->getName() + " n'est plus brûlé !\n";
            break;

        case LOST_PAR:
            s += std::string("L'ennemi ") + this->adv[i]->getName() + " n'est plus paralysé !\n";
            break;

        case LOST_PSN:
            s += std::string("L'ennemi ") + this->adv[i]->getName() + " n'est plus empoisonné !\n";
            break;

        default:
            break;
        }
    }
    if (s != "")
        this->action.setString(s);
    for (int i=0; i < this->equip->getSize(); ++i)
    {
        int r = this->equip->getCrea(i)->updateState();
        switch(r)
        {
        case DEAD_BUR:
            s += this->equip->getCrea(i)->getName() + " est mort de sa brulure\n";
            break;

        case DEAD_PSN:
            s += this->equip->getCrea(i)->getName() + " est mort empoisonné\n";
            break;

        case LOST_BUR:
            s += this->equip->getCrea(i)->getName() + " n'est plus brûlé !\n";
            break;

        case LOST_PAR:
            s += this->equip->getCrea(i)->getName() + " n'est plus paralysé !\n";
            break;

        case LOST_PSN:
            s += this->equip->getCrea(i)->getName() + " n'est plus empoisonné !\n";
            break;

        default:
            break;
        }
    }
    if (s != "")
        this->action.setString(s);

    if (s != "")
        this->wait = 240;
}

void FightView::attack(int selected, int index_my_creatures)
{
    // our creature attacking
    Creature* my = this->equip->getCrea(index_my_creatures);

    this->display_attack = true;
    DebugLog(SH_INFO, "display_attack=true");
    this->attack_frames_count = FightView::ATK_FR_CNT();

    int temp_sel = (selected == 42) ? 1 : selected;

    if (this->attacking_enemy)
    {
        this->particles.setColor(sf::Color::Blue);
        this->eq_x = this->sprites[this->__adv + to_string<int>(selected)].getPosition().x + this->sprites[this->__adv + to_string<int>(selected)].getGlobalBounds().width / 2.0f;
        this->eq_y = this->sprites[this->__adv + to_string<int>(selected)].getPosition().y + CREATURE_HEIGHT / 2.0f;
    }
    else
    {
        this->particles.setColor(sf::Color::Green);
        this->eq_x = this->sprites[this->__me + to_string<int>(index_my_creatures)].getPosition().x + this->sprites[this->__me + to_string<int>(index_my_creatures)].getGlobalBounds().width / 2.0f;
        this->eq_y = this->sprites[this->__me + to_string<int>(index_my_creatures)].getPosition().y + CREATURE_HEIGHT / 2.0f;
    }

    if (selected == 42) // magic code, we need to select random creatures
    {
        this->action.setString(my->getName() + " attaque");

        int targets = my->getSort()->getTargets();
        std::vector<int> enemies;
        int m = (this->attacking_enemy) ? this->adv.size() : this->equip->getSize();
        int real_targets = (m > targets) ? targets : m;

        for (int i=0; i < real_targets; ++i)
        {
            int r = rand() % m;
            /// we check if r is already in the vector otherwise we must change it
            for (int k=0; k < enemies.size(); ++k)
            {
                if (enemies[k] == r)
                {
                    r = rand() % m;
                    k = 0;
                }
            }
            enemies.push_back(r);
        }
        for (auto& e : enemies)
        {
            if (this->attacking_enemy)
            {
                this->cibles.push_back(e);
                my->attack(this->adv[e]);
            }
            else
            {
                this->cibles.push_back(-e - 1);
                my->attack(this->equip->getCrea(e));
            }
        }
    }
    else
    {
        // the enemy
        Creature* other;
        if (this->attacking_enemy)
        {
            other = this->adv[selected];
            this->action.setString(my->getName() + " attaque " + other->getName());
            this->cibles.push_back(selected);
        }
        else
        {
            // a friend
            other = this->equip->getCrea(selected);
            this->action.setString(my->getName() + " aide " + other->getName());
            this->cibles.push_back(-selected - 1);
        }
        my->attack(other);
    }
}

void FightView::e_attack(int selected)
{
    // enemy
    Creature* my = this->adv[selected % this->adv.size()];

    this->display_attack = true;
    DebugLog(SH_INFO, "display_attack=true");
    this->attack_frames_count = FightView::ATK_FR_CNT();

    int targets = my->getSort()->getTargets();
    SortilegeType s = my->getSort()->getType();
    bool us = (s == UniqueTargetUsHeal) || (s == MultipleUsHeal) || (s == MultipleUsHealStatus);

    int temp_sel = (targets > 1) ? 1 : selected;
    if (us)
    {
        this->particles.setColor(sf::Color::Yellow);
        this->eq_x = this->sprites[this->__adv + to_string<int>(selected % this->adv.size())].getPosition().x + this->sprites[this->__adv + to_string<int>(selected % this->adv.size())].getGlobalBounds().width / 2.0f;
        this->eq_y = this->sprites[this->__adv + to_string<int>(selected % this->adv.size())].getPosition().y + CREATURE_HEIGHT / 2.0f;
    }
    else
    {
        this->particles.setColor(sf::Color::Red);
        if (targets > 1)
        {
            this->eq_x = this->sprites[this->__me + to_string<int>(0)].getPosition().x + (this->sprites[this->__me + to_string<int>(0)].getGlobalBounds().width / 2.0f) * this->equip->getSize();
            this->eq_y = this->sprites[this->__me + to_string<int>(0)].getPosition().y + CREATURE_HEIGHT / 2.0f;
        }
    }

    if (targets > 1)
    {
        this->action.setString(std::string("L'ennemi ") + my->getName() + " attaque");

        std::vector<int> enemies;
        int m = (us) ? this->adv.size() : this->equip->getSize();
        int real_targets = (m > targets) ? targets : m;

        for (int i=0; i < real_targets; ++i)
        {
            int r = rand() % m;
            /// we must check if the enemy chosen was already chosen or not to choose another one if so
            for (int k=0; k < enemies.size(); ++k)
            {
                if (enemies[k] == r)
                {
                    r = rand() % m;
                    k = 0;
                }
            }
            enemies.push_back(r);
        }

        for (auto& e : enemies)
        {
            if (us)
            {
                this->cibles.push_back(e);
                my->attack(this->adv[e]);
            }
            else
            {
                this->cibles.push_back(-e - 1);
                my->attack(this->equip->getCrea(e));
            }
        }
    }
    else
    {
        // the enemy
        if (!us)
        {
            int n = rand() % this->equip->getSize();

            this->eq_x = this->sprites[this->__me + to_string<int>(n)].getPosition().x + this->sprites[this->__me + to_string<int>(n)].getGlobalBounds().width / 2.0f;
            this->eq_y = this->sprites[this->__me + to_string<int>(n)].getPosition().y + CREATURE_HEIGHT / 2.0f;

            Creature* enemy = this->equip->getCrea(n);
            /// we must take a living creature !
            while (enemy->getLife() == 0)
            {
                n = rand() % this->equip->getSize();
                enemy = this->equip->getCrea(n);
            }
            this->action.setString(std::string("L'ennemi ") + my->getName() + " attaque " + enemy->getName());
            my->attack(enemy);
            this->cibles.push_back(-n - 1);
        }
        else
        {
            int n = rand() % this->adv.size();
            Creature* enemy = this->adv[n];
            /// take only a living creature
            while (enemy->getLife() == 0)
            {
                n = rand() % this->adv.size();
                enemy = this->adv[n];
            }
            this->action.setString(std::string("L'ennemi ") + my->getName() + " aide son allié " + enemy->getName());
            my->attack(enemy);
            this->cibles.push_back(n);
        }
    }
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
    this->ending = 0;
    this->__c = 0;
    this->__selected = -1;
    this->selectingcrea = false;
    this->selectingadv = true;
    this->__count_before_flyaway = 0;
    /// this->can_escape = true;
    this->ui_my_selected = 0;
    this->ui_enemy_selected = 0;
    this->attacking = false;
    this->has_selected_an_atk = false;
    this->atk_using_sort_of = -1;
    this->attack_frames_count = 0;
    this->display_attack = false;
    this->my_turn = true;
    this->attacking_enemy = true;
    this->enemy_wait_until_next = 0;
    this->enemy_is_attacking = false;
    this->lock = false;
    this->action.setString("C'est à votre tour d'attaquer !");
    this->wait_give_xp = 0;
    this->whoisdead = NODEAD;
    this->iamattacking = false;
    this->wait = 0;
    this->giving_xp_to = -1;
    this->eq_x = 0.0f;
    this->eq_y = 0.0f;
    sf::Uint8* pixels = new sf::Uint8[WIN_W * WIN_W * 4];
    for (unsigned int i=0; i < WIN_W * WIN_H * 4; ++i) { pixels[i] = 0; }
    this->black_fade.update(pixels);
    this->black_fade_sprite.setTexture(this->black_fade);
    this->black_fade_sprite.setColor(sf::Color(0, 0, 0, 255));
    this->particles.update(sf::seconds(3.0f));

    this->attacks_used.clear();
    this->attacks_used.reserve(this->equip->getSize());
    for (int i=0; i < this->equip->getSize(); i++)
    {
        this->attacks_used.push_back(false);
    }
    this->my_turn = true;
    DebugLog(SH_INFO, this->equip->getSize() << " " << this->attacks_used.size());

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
        int width = this->sprites[this->__me + to_string<int>(i)].getGlobalBounds().width,
             height = this->sprites[this->__me + to_string<int>(i)].getGlobalBounds().height;
        // reversing our creatures to make them look to the right (to the others creatures)
        this->sprites[this->__me + to_string<int>(i)].setScale(-factor, factor);
        this->sprites[this->__me + to_string<int>(i)].setOrigin(width, 0.0f);
        // calculate position of the sprite regarding to the index
        this->sprites[this->__me + to_string<int>(i)].setPosition(47.0f + i * SPACEMENT_X, 458.0f - CREATURE_HEIGHT);
    }
    moy_equip /= float(this->equip->getSize());

    int _x_ = (moy_equip - 2 > 0) ? moy_equip - 2 : moy_equip;

    if (this->random_encounter || (!this->random_encounter && this->_opponents.empty()))
    {
        // generate adv
        for (int i=0; i < this->equip->getMaxSize(); i++)
        {
            Creature* crea = new Creature();
            int  id = rand() % (this->dex->getMaxId())
                  , _t = this->dex->getInfo(id).type
                  , _st = rand() % SortilegeType::Count  // SortilegeType::UniqueTargetAdvDamage
                  , level = (rand() % 6) + _x_ + 1
                  , life = 2 * level + (rand() % 4) + 3  // mlife = life
                  , pp = Creature::calculatePPFromLevel(level) // mpp = pp
                  , sdmg = ceil(((rand() % 4) + 3) * 0.125 * level + 1)  // damages for the sortilege
                  , stargets = rand() % (this->equip->getSize()+ 1)  // targets for the sortilege
                  , atk = Creature::calculateStatFromLevel(level)  // attack of the creature
                  , def = Creature::calculateStatFromLevel(level);  // defense
            long int exp = Creature::calculateExpFromLevel(level);

            Type t = static_cast<Type>(_t % 8);
            SortilegeType st = static_cast<SortilegeType>(_st % 14);

            crea->load(id, t, atk, def, life, life, pp, pp, this->dex->getName(id), State::STD, level, exp, st, sdmg, stargets);
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
    }
    else
    {
        // random_encouter == false && !this->_opponents.empty()
        // generate adv, but based on the opponents given :D
        for (int i=0; i < this->_opponents.size(); i++)
        {
            fight_opponent cur = this->_opponents[i];
            Creature* crea = new Creature();

            int  id = this->_opponents[i].id
                  , _t = this->dex->getInfo(id).type
                  , _st = (cur.sort_type == -1) ? rand() % SortilegeType::Count : cur.sort_type  // SortilegeType::UniqueTargetAdvDamage
                  , level = (cur.lvl == -1) ? (rand() % 6) + _x_ + 1 : cur.lvl
                  , life = (cur.life == -1) ? 2 * level + (rand() % 4) + 3 : cur.life  // mlife = life
                  , pp = Creature::calculatePPFromLevel(level) // mpp = pp
                  , sdmg = (cur.sort_dmg == -1) ? ceil(((rand() % 4) + 3) * 0.125 * level + 1) : cur.sort_dmg  // damages for the sortilege
                  , stargets = (cur.sort_targets == -1) ? rand() % (this->equip->getSize()+ 1) : cur.sort_targets  // targets for the sortilege
                  , atk = (cur.atk == -1) ? Creature::calculateStatFromLevel(level) : cur.atk  // attack of the creature
                  , def = (cur.def == -1) ? Creature::calculateStatFromLevel(level) : cur.def;  // defense
            long int exp = Creature::calculateExpFromLevel(level);

            Type t = static_cast<Type>(_t % 8);
            SortilegeType st = static_cast<SortilegeType>(_st % 14);

            crea->load(id, t, atk, def, life, life, pp, pp, this->dex->getName(id), State::STD, level, exp, st, sdmg, stargets);
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

void FightView::set_random_encounter(bool re)
{
    this->random_encounter = re;
}

void FightView::on_end()
{
    this->random_encounter = true;
    this->_opponents.clear();
}

void FightView::set_opponents(std::vector<fight_opponent> vfos)
{
    this->_opponents = vfos;
}

int FightView::ATK_FR_CNT()
{
    return Config::get("fps").asFloat() * 1.8f;
}
