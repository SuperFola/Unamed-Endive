#include "../../debug.hpp"
#include <iostream>

#include "../constants.hpp"
#include "../abstract/defines.hpp"

#include "settings.hpp"

SettingsView::SettingsView() :
    View(SETTINGS_VIEW_ID)
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
    ctrl.setString("Controles");
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
    this->texts.add(this->CTRL, ctrl);
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

    return true;
}

void SettingsView::update_texts()
{
    this->texts.get(this->VMU).setString((Config::get("music").asBool()) ? "on" : "off");
    this->texts.get(this->VAA).setString((Config::get("aa").asInt() > 0) ? "on" : "off");
    this->texts.get(this->VVS).setString((Config::get("v-sync").asBool()) ? "on" : "off");
    this->texts.get(this->VSHADER).setString(Config::get("shader").asString());
    this->texts.get(this->VFPS).setString(to_string<int>(Config::get("fps").asInt()));
    this->texts.get(this->VMENU).setString(Config::get("menu").asString());
    this->texts.get(this->VUP).setString(Config::get("up").asString());
    this->texts.get(this->VDW).setString(Config::get("down").asString());
    this->texts.get(this->VRI).setString(Config::get("right").asString());
    this->texts.get(this->VLE).setString(Config::get("left").asString());
}

void SettingsView::render(sf::RenderWindow& window)
{
    window.draw(this->sprites[this->BCKG]);

    window.draw(this->texts.get(this->MUSIC));
    window.draw(this->texts.get(this->AA));
    window.draw(this->texts.get(this->VS));
    window.draw(this->texts.get(this->SHADER));
    window.draw(this->texts.get(this->FPS));
    window.draw(this->texts.get(this->CTRL));
    window.draw(this->texts.get(this->MENU));
    window.draw(this->texts.get(this->UP));
    window.draw(this->texts.get(this->DW));
    window.draw(this->texts.get(this->RI));
    window.draw(this->texts.get(this->LE));

    // display also what it is binded to
}

int SettingsView::process_event(sf::Event& event, sf::Time elapsed)
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

void SettingsView::update(sf::RenderWindow& window, sf::Time elapsed)
{
    /// faudra faire ca quelque part quand on change les settings :
    window.setVerticalSyncEnabled(Config::get("v-sync").asBool());
    window.getSettings().antialiasingLevel = Config::get("aa").asInt();
    window.setFramerateLimit(Config::get("fps").asInt());
}
