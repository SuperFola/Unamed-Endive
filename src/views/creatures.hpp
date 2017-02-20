#ifndef DEF_CREA_VIEW
#define DEF_CREA_VIEW

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

#include "view.hpp"
#include "../abstract/texturesmanager.hpp"
#include "../abstract/equip.hpp"
#include "../abstract/functions.hpp"
#include "../abstract/creatures_loader.hpp"
#include "../objects/dex.hpp"

class CreaView : public View
{
private:
    // variables
    bool displaying_crea;
    TexturesManager textures;
    std::map<std::string, sf::Sprite> sprites;
    sf::Font font;
    Equip* equip;
    Equip* pc;
    Dex* dex;
    CreaturesLoader* creaload;

    sf::Text lsdata;
    sf::Text cnam, clev, ctyp, csta, catk, cdef, clif;
    sf::Sprite cimg;

    int selected;
    int offset;

    const std::string BCKG = "background";
    const std::string BCKG_PC = "background_pc";
    const std::string BTN_PC = "btn_pc";
    const std::string BTN_CREA = "btn_crea";

    // methods
    void draw_content(sf::RenderWindow&);
    void send_to(int);
    void set_cimg(int);

public:
    // methods
    CreaView();
    bool load() override;
    void post_load();
    void render(sf::RenderWindow&) override;
    int process_event(sf::Event&, sf::Time) override;
    void update(sf::RenderWindow&, sf::Time) override;
    void add_equip(Equip*);
    void add_pc(Equip*);
    void add_creaload(CreaturesLoader*);
    void add_dex(Dex*);
};

#endif // DEF_CREA_VIEW



