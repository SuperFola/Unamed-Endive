#ifndef DEF_DEFAULT_VIEW
#define DEF_DEFAULT_VIEW

#include "view.hpp"
#include "../map/map.hpp"
#include "../particles/particles.hpp"
#include "../entities/character.hpp"

class DefaultView : public View
{
private:
    // variables
    sf::VertexArray triangle;
    ParticleSystem particles;
    Character player;
    Map mymap;

public:
    // methods
    DefaultView();
    void render(sf::RenderWindow&) override;
    int process_event(sf::Event&) override;
    void update(sf::RenderWindow&, sf::Time) override;
};

#endif // DEF_DEFAULT_VIEW
