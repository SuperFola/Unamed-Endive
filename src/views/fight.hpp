#ifndef DEF_FIGHT_VIEW
#define DEF_FIGHT_VIEW

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

#include "view.hpp"
#include "../abstract/container.hpp"
#include "../constants.hpp"
#include "../objects/dex.hpp"
#include "../entities/creature.hpp"
#include "../abstract/equip.hpp"
#include "../abstract/creatures_loader.hpp"
#include "../abstract/defines.hpp"

#define X_TEXT_SELCREA_UI 200
#define MX_TEXT_SELCREA_UI 400
#define Y_TEXT_SELCREA_UI 200
#define MY_TEXT_SELCREA_UI 400
#define YS_TEXT_SELCREA_UI 30
#define CREATURE_HEIGHT 160.0f

class FightView : public View
{
private:
    Container<sf::Texture> textures;
    std::map<std::string, sf::Sprite> sprites;
    Container<sf::Text> texts;
    std::vector<Creature*> adv;
    sf::Font font;
    sf::Text action;
    sf::Text enemy;
    sf::Text me;
    sf::Text e_pv;
    sf::Text m_pv;
    FightEnv env;
    Dex* dex;
    Equip* equip;
    Equip* pc;
    CreaturesLoader* crealoader;
    int __c;        // for the capture
    int __selected;  // for the UI when we select an enemy
    bool selectingcrea; // are we selected a creature (change it)
    bool selectingadv; // or the enemy (to attack it)
    int __count_before_flyaway;  // count down in frames
    bool can_escape;
    sf::RectangleShape life1;
    sf::RectangleShape life2;
    int ui_my_selected;     // to know which creature we are currently displaying the stats
    int ui_enemy_selected;  // same

    const std::string __adv = "adv";
    const std::string __me = "me";

    const std::string BKG1 = "bkg1";
    const std::string BKG2 = "bkg2";
    const std::string BKG3 = "bkg3";
    const std::string GRD1 = "grd1";
    const std::string GRD2 = "grd2";
    const std::string GRD3 = "grd3";
    const std::string TOOLS = "toolbar";
    const std::string LIFEBAR = "lifebar";
    const std::string LIFEBAR2 = "lifebar2";
    const std::string OVERLAY = "overlay";
    const std::string BKG_SELECT = "background_select";

public:
    FightView();
    ~FightView();
    bool load() override;
    void render(sf::RenderWindow&) override;
    int process_event(sf::Event&, sf::Time) override;
    void update(sf::RenderWindow&, sf::Time) override;
    void encounter();
    void set_env(FightEnv);
    void set_dex(Dex*);
    void set_equip(Equip*);
    void set_pc(Equip*);
    void set_crealoader(CreaturesLoader*);
    void start();
    void set_escape(bool);
};

#endif // DEF_FIGHT_VIEW
