#ifndef DEF_FIGHT_VIEW
#define DEF_FIGHT_VIEW

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

#include "view.hpp"
#include "../abstract/texturesmanager.hpp"
#include "../constants.hpp"

class FightView : public View
{
private:
    TexturesManager textures;
    std::map<std::string, sf::Sprite> sprites;
    sf::Font font;
    sf::Text action;
    sf::Text ennemy;
    sf::Text me;
    FightEnv env;

    const std::string BKG1 = "bkg1";
    const std::string BKG2 = "bkg2";
    const std::string BKG3 = "bkg3";
    const std::string GRD1 = "grd1";
    const std::string GRD2 = "grd2";
    const std::string GRD3 = "grd3";
    const std::string TOOLS = "toolbar";
    const std::string LIFEBAR = "lifebar";
    const std::string OVERLAY = "overlay";

public:
    FightView();
    bool load() override;
    void render(sf::RenderWindow&) override;
    int process_event(sf::Event&, sf::Time) override;
    void update(sf::RenderWindow&, sf::Time) override;
    void set_env(FightEnv);
};

#endif // DEF_FIGHT_VIEW
