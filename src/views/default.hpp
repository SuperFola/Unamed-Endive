#ifndef DEF_DEFAULT_VIEW
#define DEF_DEFAULT_VIEW

#include <tuple>
#include <string>
#include <SFML/System/String.hpp>

#include "view.hpp"
#include "../map/map.hpp"
#include "../entities/pnj.hpp"
#include "huds/menu.hpp"
#include "../constants.hpp"
#include "../entities/pnjmanager.hpp"
#include "../abstract/functions.hpp"
#include "../json/json.h"
#include "../entities/character.hpp"

std::vector<std::string> glob_frag(const std::string& directory);

class DefaultView : public View
{
private:
    // variables
    sf::View view;
    sf::RenderTexture offscreen;
    sf::RenderTexture minimap;
    sf::RenderTexture world;
    sf::Sprite offsprite;
    sf::Sprite minisprite;
    sf::Sprite worldsprite;
    Character player;
    Map level;
    PNJManager pnjmgr;
    MenuHUD menu_hud;
    bool display_mmap;
    std::tuple <bool, int, int> _speaking_to_pnj;
    std::vector<std::string> shaders;
    std::string current_shader;
    sf::Shader shader;
    sf::Texture distortionMap;
    // methods
    void set_view(sf::RenderWindow&);
    void unset_view(sf::RenderWindow&);

    const std::string shaders_path = "assets/shaders/";

public:
    // methods
    DefaultView();
    bool load(sf::String);
      // we are overriding it but not using it
      bool load() override;
    void render(sf::RenderWindow&) override;
        // we are overriding it but not using it
        int process_event(sf::Event&, sf::Time) override;
    int process_event(sf::Event&, sf::Time, sf::RenderWindow&);
    void update(sf::RenderWindow&, sf::Time) override;
    bool hasActiveHud();
    Character* getCharacter();
    PNJManager* getPNJM();
    Map* getMap();
    void change_display_mmap(bool);
    bool get_display_mmap();
    void draw_on_offscreen(const sf::Drawable&);
    void resolve_pnjspeak(int, int, sf::RenderWindow&);
    void disable_pnj_speaking();

    template <typename T>
    void setShaderParameter(const std::string& param, T& value)
    {
        if (this->current_shader != "")
            this->shader.setUniform(param, value);
    }
    void setShader(const std::string& name);
    const std::string& getCurrentShader();
};

#endif // DEF_DEFAULT_VIEW
