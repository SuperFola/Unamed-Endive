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

class FightView : public View
{
private:
    Container<sf::Texture> textures;
    std::map<std::string, sf::Sprite> sprites;
    std::vector<Creature> adv;
    sf::Font font;
    sf::Text action;
    sf::Text ennemy;
    sf::Text me;
    FightEnv env;
    Dex* dex;

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
    void encounter();
    void set_env(FightEnv);
    void set_dex(Dex*);
};

#endif // DEF_FIGHT_VIEW
