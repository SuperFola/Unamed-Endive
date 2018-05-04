#include "../../debug.hpp"
#include <iostream>

#include "../constants.hpp"
#ifdef PLATFORM_WIN
    #include <windows.h>
#endif // PLATFORM_WIN
#include "../abstract/defines.hpp"

#include "settings.hpp"

SettingsView::SettingsView() :
    View(SETTINGS_VIEW_ID)
    , key_needed(k_none)
{

}

bool SettingsView::load()
{
    // textures
    sf::Texture bckg;
    if (!bckg.loadFromFile("assets/gui/settings/fd_settings.png"))
        return false;
    sf::Texture checked;
    if (!checked.loadFromFile("assets/gui/settings/fd_checked.png"))
        return false;
    sf::Texture unchecked;
    if (!unchecked.loadFromFile("assets/gui/settings/fd_unchecked.png"))
        return false;

    this->textures.add(this->BCKG, bckg);
    this->textures.add(this->CHECKED, checked);
    this->textures.add(this->UNCHECKED, unchecked);

    // setting sprites
    this->sprites[this->BCKG] = sf::Sprite(this->textures.get(this->BCKG));
    this->sprites[this->CHECKED] = sf::Sprite(this->textures.get(this->CHECKED));
    this->sprites[this->UNCHECKED] = sf::Sprite(this->textures.get(this->UNCHECKED));

    // setting positions
    this->sprites[this->BCKG].setPosition(0.0f, 0.0f);

    // font
    if (!this->font.loadFromFile("assets/fonts/pkmnemn.ttf"))
        return false;

    // setting texts
    sf::Text music;
    setupFont(music, this->font, sf::Color::White, 18)
    music.setString("Musique");
    sf::Text aa;
    setupFont(aa, this->font, sf::Color::White, 18)
    aa.setString("Anti-aliasing");
    #ifndef BUILD_WITH_OGL
        aa.setStyle(sf::Text::StrikeThrough);
    #endif // BUILD_WITH_OGL
    sf::Text vs;
    setupFont(vs, this->font, sf::Color::White, 18)
    vs.setString("V-Sync");
    sf::Text sh;
    setupFont(sh, this->font, sf::Color::White, 18)
    sh.setString("Shader");
    sf::Text fps;
    setupFont(fps, this->font, sf::Color::White, 18)
    fps.setString("FPS");
    sf::Text ctrl;
    setupFont(ctrl, this->font, sf::Color::White, 18)
    ctrl.setString("Contrôles");
    sf::Text menu;
    setupFont(menu, this->font, sf::Color::White, 18)
    menu.setString("Menu");
    sf::Text up;
    setupFont(up, this->font, sf::Color::White, 18)
    up.setString("Avancer");
    sf::Text dw;
    setupFont(dw, this->font, sf::Color::White, 18)
    dw.setString("Reculer");
    sf::Text ri;
    setupFont(ri, this->font, sf::Color::White, 18)
    ri.setString("Aller à droite");
    sf::Text le;
    setupFont(le, this->font, sf::Color::White, 18)
    le.setString("Aller à gauche");
    // values
    sf::Text v_mu;
    setupFont(v_mu, this->font, sf::Color::White, 18)
    sf::Text v_aa;
    setupFont(v_aa, this->font, sf::Color::White, 18)
    sf::Text v_vs;
    setupFont(v_vs, this->font, sf::Color::White, 18)
    sf::Text v_shader;
    setupFont(v_shader, this->font, sf::Color::White, 18)
    sf::Text v_fps;
    setupFont(v_fps, this->font, sf::Color::White, 18)
    sf::Text v_menu;
    setupFont(v_menu, this->font, sf::Color::White, 18)
    sf::Text v_up;
    setupFont(v_up, this->font, sf::Color::White, 18)
    sf::Text v_dw;
    setupFont(v_dw, this->font, sf::Color::White, 18)
    sf::Text v_ri;
    setupFont(v_ri, this->font, sf::Color::White, 18)
    sf::Text v_le;
    setupFont(v_le, this->font, sf::Color::White, 18)

    this->texts.add(this->MUSIC, music);
    this->texts.add(this->AA, aa);
    this->texts.add(this->VS, vs);
    this->texts.add(this->SHADER, sh);
    this->texts.add(this->FPS, fps);
    this->texts.add(this->MENU, menu);
    this->texts.add(this->UP, up);
    this->texts.add(this->DW, dw);
    this->texts.add(this->RI, ri);
    this->texts.add(this->LE, le);
    // values
    this->texts.add(this->VMU, v_mu);
    this->texts.add(this->VAA, v_aa);
    this->texts.add(this->VVS, v_vs);
    this->texts.add(this->VSHADER, v_shader);
    this->texts.add(this->VFPS, v_fps);
    this->texts.add(this->VMENU, v_menu);
    this->texts.add(this->VUP, v_up);
    this->texts.add(this->VDW, v_dw);
    this->texts.add(this->VRI, v_ri);
    this->texts.add(this->VLE, v_le);

    this->update_texts();

    // setting texts position
    this->texts.get(this->MUSIC).setPosition(SET_START_X, SET_START_Y);
    this->texts.get(this->AA).setPosition(SET_START_X, SET_START_Y + SET_SPACE_Y);
    this->texts.get(this->VS).setPosition(SET_START_X, SET_START_Y + 2.0f * SET_SPACE_Y);
    this->texts.get(this->SHADER).setPosition(SET_START_X, SET_START_Y + 3.0f * SET_SPACE_Y);
    this->texts.get(this->FPS).setPosition(SET_START_X, SET_START_Y + 4.0f * SET_SPACE_Y);
    this->texts.get(this->MENU).setPosition(SET_START_X, SET_START_Y + 5.0f * SET_SPACE_Y);
    this->texts.get(this->UP).setPosition(SET_START_X, SET_START_Y + 6.0f * SET_SPACE_Y);
    this->texts.get(this->DW).setPosition(SET_START_X, SET_START_Y + 7.0f * SET_SPACE_Y);
    this->texts.get(this->RI).setPosition(SET_START_X, SET_START_Y + 9.0f * SET_SPACE_Y);
    this->texts.get(this->LE).setPosition(SET_START_X, SET_START_Y + 8.0f * SET_SPACE_Y);

    this->texts.get(this->VMU).setPosition(SET_START_X2, SET_START_Y);
    this->texts.get(this->VAA).setPosition(SET_START_X2, SET_START_Y + SET_SPACE_Y);
    this->texts.get(this->VVS).setPosition(SET_START_X2, SET_START_Y + 2.0f * SET_SPACE_Y);
    this->texts.get(this->VSHADER).setPosition(SET_START_X2, SET_START_Y + 3.0f * SET_SPACE_Y);
    this->texts.get(this->VFPS).setPosition(SET_START_X2, SET_START_Y + 4.0f * SET_SPACE_Y);
    this->texts.get(this->VMENU).setPosition(SET_START_X2, SET_START_Y + 5.0f * SET_SPACE_Y);
    this->texts.get(this->VUP).setPosition(SET_START_X2, SET_START_Y + 6.0f * SET_SPACE_Y);
    this->texts.get(this->VDW).setPosition(SET_START_X2, SET_START_Y + 7.0f * SET_SPACE_Y);
    this->texts.get(this->VRI).setPosition(SET_START_X2, SET_START_Y + 9.0f * SET_SPACE_Y);
    this->texts.get(this->VLE).setPosition(SET_START_X2, SET_START_Y + 8.0f * SET_SPACE_Y);

    return true;
}

void SettingsView::update_texts()
{
    this->texts.get(this->VMU).setString((Config::get("music").asBool()) ? "on" : "off");
    this->texts.get(this->VAA).setString((Config::get("aa").asInt() > 0) ? "on" : "off");
    this->texts.get(this->VVS).setString((Config::get("v-sync").asBool()) ? "on" : "off");
    this->texts.get(this->VSHADER).setString(Config::get("shader").asString());
    this->texts.get(this->VFPS).setString(to_string<int>(Config::get("fps").asInt()));

    if (this->key_needed != k_menu)
        this->texts.get(this->VMENU).setString(Config::get("menu").asString());
    else
        this->texts.get(this->VMENU).setString("> ??? <");

    if (this->key_needed != k_up)
        this->texts.get(this->VUP).setString(Config::get("up").asString());
    else
        this->texts.get(this->VUP).setString("> ??? <");

    if (this->key_needed != k_dwn)
        this->texts.get(this->VDW).setString(Config::get("down").asString());
    else
        this->texts.get(this->VDW).setString("> ??? <");

    if (this->key_needed != k_ri)
        this->texts.get(this->VRI).setString(Config::get("right").asString());
    else
        this->texts.get(this->VRI).setString("> ??? <");

    if (this->key_needed != k_le)
        this->texts.get(this->VLE).setString(Config::get("left").asString());
    else
        this->texts.get(this->VLE).setString("> ??? <");
}

void SettingsView::render(sf::RenderWindow& window)
{
    window.draw(this->sprites[this->BCKG]);

    window.draw(this->texts.get(this->MUSIC));
    window.draw(this->texts.get(this->AA));
    window.draw(this->texts.get(this->VS));
    window.draw(this->texts.get(this->SHADER));
    window.draw(this->texts.get(this->FPS));
    window.draw(this->texts.get(this->MENU));
    window.draw(this->texts.get(this->UP));
    window.draw(this->texts.get(this->DW));
    window.draw(this->texts.get(this->RI));
    window.draw(this->texts.get(this->LE));

    /*
    unuseful, we are using images instead of those =>
        window.draw(this->texts.get(this->VMU));
        window.draw(this->texts.get(this->VAA));
        window.draw(this->texts.get(this->VVS));
    */

    std::string c = (Config::get("music").asBool()) ? this->CHECKED : this->UNCHECKED;
    this->sprites[c].setPosition(435.0f, 82.0f);
    window.draw(this->sprites[c]);

    c = (Config::get("aa").asInt() > 0) ? this->CHECKED : this->UNCHECKED;
    this->sprites[c].setPosition(435.0f, 138.0f);
    window.draw(this->sprites[c]);

    c = (Config::get("v-sync").asBool()) ? this->CHECKED : this->UNCHECKED;
    this->sprites[c].setPosition(435.0f, 194.0f);
    window.draw(this->sprites[c]);

    window.draw(this->texts.get(this->VSHADER));
    window.draw(this->texts.get(this->VFPS));
    window.draw(this->texts.get(this->VMENU));
    window.draw(this->texts.get(this->VUP));
    window.draw(this->texts.get(this->VDW));
    window.draw(this->texts.get(this->VRI));
    window.draw(this->texts.get(this->VLE));
}

int SettingsView::process_event(sf::Event& event, sf::Time elapsed)
{
    int new_view = -1;
    int r = -1;
    std::string k = "";

    switch(event.type)
    {
    case sf::Event::TextEntered:
        // code to get a key
        if (this->key_needed != k_none)
        {
            this->key = SettingsView::convert_textentered_to_value(event.text.unicode);
        }
        else
        {
            k = SettingsView::convert_textentered_to_value(event.text.unicode);
            if (Config::get("menu") == k)  // escape only if we are not taking an input
                new_view = LAST_VIEW_ID;
        }
        break;

    case sf::Event::KeyPressed:
        if (this->key_needed != k_none)
        {
            this->key = SettingsView::convert_textentered_to_value(event.key.code, true);
        }
        break;

    case sf::Event::MouseButtonPressed:
        switch(event.mouseButton.button)
        {
        case sf::Mouse::Button::Left:
            if (138 <= m__X && m__X <= 501)
            {
                // clic on a setting thing
                r = int((m__Y - 70.0f) / 56.0f);
                if (0 <= r && r <= 9)
                {
                    // everything is fine, we clicked on something good
                    if (r == 0)
                    {
                        // music
                        Config::set("music", !Config::get("music").asBool());

                        if (Config::get("music").asBool() && this->mplayer->getVolume() == 0.0f)
                        {
                            // this will reverse the mute state
                            this->mplayer->mute();
                            DebugLog(SH_INFO, "Music unmuted from settings view");
                        }
                        else if (!Config::get("music").asBool() && this->mplayer->getVolume() != 0.0f)
                        {
                            this->mplayer->mute();
                            DebugLog(SH_INFO, "Music muted from settings view");
                        }
                    }
                    else if (r == 1)
                    {
                        // aa
                        Config::set("aa", ((Config::get("aa").asInt() > 0) ? 0 : 1));
                    }
                    else if (r == 2)
                    {
                        // vsync
                        Config::set("v-sync", ! Config::get("v-sync").asBool());
                    }
                    else if (r == 3)
                    {
                        // shader
                        // Config::get("shader").asString()
                    }
                    else if (r == 4)
                    {
                        // fps
                        if (Config::get("fps").asInt() == 240)
                            Config::set("fps", 60);
                        else if (Config::get("fps").asInt() == 120)
                            Config::set("fps", 240);
                        else if (Config::get("fps").asInt() == 60)
                            Config::set("fps", 120);
                    }
                    else if (r == 5)
                    {
                        // menu key
                        this->key_needed = k_menu;
                    }
                    else if (r == 6)
                    {
                        // up key
                        this->key_needed = k_up;
                    }
                    else if (r == 7)
                    {
                        // down key
                        this->key_needed = k_dwn;
                    }
                    else if (r == 9)
                    {
                        // right key
                        this->key_needed = k_ri;
                    }
                    else if (r == 8)
                    {
                        // left key
                        this->key_needed = k_le;
                    }
                }
            }
            break;

        default:
            break;
        }
        // we save only if we are not asking for a key, otherwise we will save "> ??? <" as a key for an action
        // resulting in a not working action when trying to call the "chosen" key
        if (this->key_needed == k_none)
            Config::save();
        break;

    default:
        break;
    }

    return new_view;
}

void SettingsView::update(sf::RenderWindow& window, sf::Time elapsed)
{
    window.setVerticalSyncEnabled(Config::get("v-sync").asBool());

    #ifdef BUILD_WITH_OGL
    if (Config::get("aa").asInt() > 0)
        glEnable(GL_MULTISAMPLE_ARB);
    else
        glDisable(GL_MULTISAMPLE_ARB);
    #endif // BUILD_WITH_OGL

    window.setFramerateLimit(Config::get("fps").asInt());

    this->update_texts();

    if (this->key_needed != k_none && this->key != "" && this->key != NOKEY)
    {
        std::string v = "";
        switch (this->key_needed)
        {
        case k_menu:
            v = "menu";
            break;

        case k_up:
            v = "up";
            break;

        case k_dwn:
            v = "down";
            break;

        case k_ri:
            v = "right";
            break;

        case k_le:
            v = "left";
            break;
        }

        Config::set(v, this->key);

        this->key_needed = k_none;
        this->key = "";
    }
}

void SettingsView::set_music_player(MusicPlayer* mp)
{
    this->mplayer = mp;
}

std::string SettingsView::convert_textentered_to_value(sf::Uint32 e, bool kp)
{
    sf::String s;
    if (!kp)
    {
        // handling special cases
        if (e == '\b')
            s.insert(0, "retour");
        else if (e == 13 || e == 10)
            s.insert(0, "entree");
        else if (e == 27)
            s.insert(0, "escape");
        else
            s.insert(0, e);  // otherwise just add the character as a utf 8 one
    }
    else
    {
        if (e == sf::Keyboard::Up)
            s.insert(0, "haut");
        else if (e == sf::Keyboard::Down)
            s.insert(0, "bas");
        else if (e == sf::Keyboard::Right)
            s.insert(0, "droite");
        else if (e == sf::Keyboard::Left)
            s.insert(0, "gauche");
        else
            s.insert(0, "");
    }
    return s.toAnsiString();
}
